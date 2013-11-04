#include <config.hpp>
#include <cstring>
#include <ctime>
#include <deque>
#include <utility>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/util/inet.hpp>
#include <muddy/common/util/misc.hpp>
#include <muddy/common/wrapper/wrapper.hpp>
#include <muddy/common/algorithm/HashMap.hpp>
#include <muddy/common/Configuration.hpp>
#include <muddy/common/InitManager.hpp>
#include <muddy/common/inet/DatagramSocket.hpp>
#include <muddy/common/thread/sync.hpp>
#include <muddy/common/thread/Thread.hpp>
#include <muddy/adapter/Tuntap.hpp>
#include <muddy/tunnel/Cryptographer.hpp>
#include <muddy/tunnel/protocol.hpp>

using namespace muddy;

namespace {

char* splitNetworkString(char* network) {
	char* slash = std::strchr(network, '/');
	if (slash == NULL) {
		throw ConfigError("invalid network %s", network);
	}
	*slash = '\0';
	++slash;
	if (*slash == '\0') {
		throw ConfigError("network id missing %s", network);
	}
	return slash;
}

in_addr_t getNetMask(const char* prefix) {
	int size;
	if (std::sscanf(prefix, " %d ", &size) != 1
			|| size < 1 || size > 31) {
		throw ConfigError("invalid network prefix \"%s\"", prefix);
	}
	return (~(in_addr_t)0) << (32 - size);
}

#if !_WIN32
void dropRoot() {
	passwd* pw = getpwnam("nobody");
	if (pw == NULL) {
		LOG_WARNING << "Cannot find system user \"nobody\"";
		return;
	}
	wrapper::setgid_(pw->pw_gid);
	wrapper::setuid_(pw->pw_uid);
}
#endif

class MuddyServer {
public:
	MuddyServer(Tuntap* t, DatagramSocket* s, const char* sec,
			uint32_t begin, uint32_t end)
			: tuntap(t), socket(s), inbound(NULL), outbound(NULL),
			keepGoing(true), crypto(sec) {
		client.pool = new ClientInfo[end - begin];
		std::memset(client.pool, 0, (end - begin) * sizeof(*client.pool));
		for (uint32_t i = begin; i < end; ++i) {
			client.pool[i].address.tunnel = htonl(i);
		}
		client.begin = client.current = begin;
		client.end = end;
	}

	void startup() {
		inbound = new Thread(&inboundThreadEntry, this);
		outbound = new Thread(&outboundThreadEntry, this);
	}

	void shutdown() {
		keepGoing = false;
		inbound->wait();
		outbound->wait();
		delete this; // TODO "delete this" is OK, but ?
	}

private:
	struct RealAddress {
		in_addr_t inet;
		uint16_t port;

		void init(const sockaddr_in& addr) {
			std::memset(this, 0, sizeof(*this));
			inet = addr.sin_addr.s_addr;
			port = addr.sin_port;
		}
	};

	struct ClientInfo {
		time_t action;
		struct {
			in_addr_t tunnel;
			RealAddress real;
		} address;
	};

	Tuntap* tuntap;
	DatagramSocket* socket;
	Thread* inbound;
	Thread* outbound;
	volatile bool keepGoing;
	Cryptographer crypto;
	struct {
		ClientInfo* pool;
		ReadWriteLock rwlock;

		uint32_t current;
		uint32_t begin, end;
		std::deque<int> free;
	} client;
	struct {
		HashMap<RealAddress, int> real;
		HashMap<in_addr_t, int> tunnel;
	} map;

	~MuddyServer() {
		delete inbound;
		delete outbound;
		delete[] client.pool;
	}

	static void inboundThreadEntry(void* arg) {
		MuddyServer* self = static_cast<MuddyServer*>(arg);
		self->handleIncomingData();
	}

	static void outboundThreadEntry(void* arg) {
		MuddyServer* self = static_cast<MuddyServer*>(arg);
		self->handleTuntapData();
	}

	static void patrolThreadEntry(void* arg) {
		MuddyServer* self = static_cast<MuddyServer*>(arg);
		self->patrolActiveClient();
	}

	void handleIncomingData();
	void handleTuntapData();
	void processTuntapFrame(const void* data, int size);
	void patrolActiveClient();
	void handlePlainText(const std::string& packet,
			const sockaddr_in& source);
	void startSession(const char* data, size_t size,
			const sockaddr_in& client);
	void endSession(const char* data, size_t size,
			const sockaddr_in& client);
	void injectData(const char* data, size_t size,
			const sockaddr_in& client);
	int allocClientInfo();
	void releaseClientInfo(int index);
	void sendData(const void* data, size_t size,
			const sockaddr_in& dest);
};

void MuddyServer::handleIncomingData() {
	enum { kMaxPacketSize = 1600 };

	std::string packet;
	while (keepGoing) {
		char buffer[kMaxPacketSize + 16];
		sockaddr_in source;
		int size = socket->read(buffer, sizeof(buffer), &source);
		if (size > kMaxPacketSize) {
			LOG_WARNING << "Packet larger than expected, ignored";
			continue;
		}
		packet.clear();
		crypto.decrypt(buffer, size, &packet);
		if (packet.size() == 0) {
			LOG_WARNING << "Failed to decrypt the packet.";
			continue;
		}
		handlePlainText(packet, source);
	}
}

void MuddyServer::handleTuntapData() {
	while (keepGoing) {
		char buffer[1600];
		int size = tuntap->read(buffer, sizeof(buffer));
		if (size < 20) {
			throw Unpossible();
		}
		processTuntapFrame(buffer, size);
	}
}

void MuddyServer::processTuntapFrame(const void* data, int size) {
	struct Header {
		uint16_t ignored1;
		uint16_t len;
		uint32_t ignored2;
		uint32_t ignored3;
		in_addr_t source;
		in_addr_t destination;
	};

	const Header* h = static_cast<const Header*>(data);
	if (size != ntohs(h->len)) {
		throw Unpossible("Invalid TUN/TAP frame");
	}

	struct Buffer {
		PacketHeader header;
		char data[1600];
	} buffer;
	buffer.header.type = kRelayData;
	buffer.header.length = size;
	std::memcpy(buffer.data, data, size);

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;

	{
		ReadGuard r(client.rwlock);
		// TODO handle broadcast address first.
		HashMap<in_addr_t, int>::Node* node =
				map.tunnel.search(h->destination);
		ClientInfo* c = client.pool + node->value;
		addr.sin_port = c->address.real.port;
		addr.sin_addr.s_addr = c->address.real.inet;
	}

	sendData(&buffer, sizeof(buffer.header) + size, addr);
}

void MuddyServer::handlePlainText(const std::string& packet,
		const sockaddr_in& source) {
	if (packet.size() < sizeof(PacketHeader)) {
		LOG_WARNING << "No packet header found, packet ignored.";
		return;
	}

	PacketHeader header;
	std::memcpy(&header, packet.c_str(), sizeof(header));

	if (packet.size() != header.length + sizeof(header)) {
		LOG_WARNING << "Wrong packet size, packet ignored.";
		return;
	}

	typedef void (MuddyServer::*PacketHandler)(const char*, size_t,
			const sockaddr_in&);
	static const PacketHandler kHandlerTable[] = {
			&MuddyServer::startSession, &MuddyServer::endSession,
			&MuddyServer::injectData
	};
	if (header.type > ELEMENT_COUNT(kHandlerTable) || header.type <= 0) {
		LOG_WARNING << "Invalid packet header.";
		return;
	}
	(this->*kHandlerTable[header.type])(packet.c_str() + sizeof(header),
			header.length, source);
}

void MuddyServer::startSession(const char* /* data */, size_t /* size */,
		const sockaddr_in& source) {
	RealAddress real;
	real.init(source);
	time_t currentTime = std::time(NULL);
	HashMap<RealAddress, int>::Node* node;
	{
		WriteGuard w(client.rwlock);
		node = map.real.search(real);
		if (node != NULL) {
			client.pool[node->value].action = currentTime;
			return;
		}
		int index = allocClientInfo();
		if (index < 0) {
			LOG_ERROR << "Too many clients";
			return;
		}
		client.pool[index].action = currentTime;
		client.pool[index].address.real = real;
		node = &map.real.insert(real);
		node->value = index;

		in_addr_t t = client.pool[index].address.tunnel;
		map.tunnel.insert(t).value = index;
	}

	struct Response {
		PacketHeader header;
		in_addr_t address;
	} r;
	r.header.type = kStartSession;
	r.header.length = sizeof(r.address);
	r.address = client.pool[node->value].address.tunnel;
	sendData(&r, sizeof(r), source);
}

void MuddyServer::endSession(const char* /* data */, size_t /* size */,
		const sockaddr_in& source) {
	RealAddress real;
	real.init(source);
	in_addr_t address;

	WriteGuard guard(client.rwlock);
	HashMap<RealAddress, int>::Node* node = map.real.search(real);
	if (node == NULL) {
		return;
	}
	int index = node->value;
	map.real.remove(client.pool[index].address.real);
	map.tunnel.remove(client.pool[index].address.tunnel);
	client.pool[index].action = 0;
	releaseClientInfo(index);
}

void MuddyServer::injectData(const char* data, size_t size,
		const sockaddr_in& source) {
	RealAddress real;
	real.init(source);
	time_t currentTime = std::time(NULL);
	HashMap<RealAddress, int>::Node* node;
	{
		ReadGuard r(client.rwlock);
		node = map.real.search(real);
		if (node == NULL) {
			return;
		}
		client.pool[node->value].action = currentTime;
	}
	tuntap->write(data, size);
}

int MuddyServer::allocClientInfo() {
	if (client.free.empty()) {
		if (client.current < client.end) {
			client.current++;
			return client.current - client.begin - 1;
		} else {
			return -1;
		}
	}
	int r = client.free.front();
	client.free.pop_front();
	return r;
}

void MuddyServer::releaseClientInfo(int index) {
	WriteGuard w(client.rwlock);
	client.free.push_back(index);
}

void MuddyServer::sendData(const void* data, size_t size,
		const sockaddr_in& dest) {
	std::string tmp;
	crypto.encrypt(static_cast<const char*>(data), size, &tmp);
	socket->write(tmp.c_str(), tmp.size(), dest);
}

} // namespace

int main(int argc, char** argv) {
	InitManager manager;

	Configuration config("server");
	config.add("listen", false, "0.0.0.0:5303").add("network", true)
			.add("secret", false, "");
#if !_WIN32
	config.add("daemon", false).add("core", false);
#endif
	config.parse(argc, argv);

#if !_WIN32
	if (util::boolOptionSet(config, "core")) {
		util::enableCoreDump();
	}
	if (util::boolOptionSet(config, "daemon")) {
		wrapper::daemon_(1, 1);
	}
#endif

	char copy[64] = { 0 };
	std::strncpy(copy, config.getString("network"), sizeof(copy) - 1);
	char* prefix = splitNetworkString(copy);
	in_addr_t netmask = getNetMask(prefix);
	in_addr_t local = wrapper::inet_addr_(copy);

	// This following two objects would be accessed by multiple threads,
	// so they cannot be created on the stack of the main thread.
	std::auto_ptr<Tuntap> tuntap(new Tuntap(local, netmask));
	std::auto_ptr<DatagramSocket> socket(new DatagramSocket());
	socket->bind(util::getInetAddress(config.getString("listen")));

#if !_WIN32
	dropRoot();
#endif
	MuddyServer* server = new MuddyServer(tuntap.get(), socket.get());
	server->startup();

	util::pause();

	server->shutdown();
}
