#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <string>
#include <thread>
#include <type_traits>
#include <muddy/veth/Tuntap.hpp>
#include <muddy/net/DatagramSocket.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>
#include <muddy/common/util/net.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/tunnel/TunnelServer.hpp>
#include <muddy/tunnel/TunnelClient.hpp>

using namespace muddy;

namespace {

struct Config {
	sockaddr_in remote;
	sockaddr_in local;
	std::string secret;
	std::string logFile;
} config;

bool parseCommandLine(int argc, char** argv) {
	std::memset(&config.remote, 0, sizeof(config.remote));
	std::memset(&config.local, 0, sizeof(config.local));
	config.secret.assign("jiubugaosuni");
	for (int i = 1; i < argc; ++i) {
		if (std::strcmp(argv[i], "--server") == 0) {
			config.remote = util::getInetAddress(argv[++i]);
		} else if (std::strcmp(argv[i], "--bind") == 0) {
			config.local = util::getInetAddress(argv[++i]);
		} else if (std::strcmp(argv[i], "--secret") == 0) {
			config.secret.assign(argv[++i]);
		} else if (std::strcmp(argv[i], "--log") == 0) {
			config.logFile.assign(argv[++i]);
		} else {
			MUDDY_ERROR << "Unknown command line: " << argv[i] << "\n";
			return false;
		}
	}
	if (config.remote.sin_port == 0
			&& config.local.sin_port == 0) {
		MUDDY_ERROR << "One of --bind/--server must be given\n";
		return false;
	} else if (config.remote.sin_port != 0
			&& config.local.sin_port != 0) {
		MUDDY_ERROR << "Cannot give --bind and --server "
				"at the same time\n";
		return false;
	}
	if (!config.logFile.empty()) {
		logging::Logger::get().setTarget(config.logFile.c_str());
	}
	return true;
}

template<typename T>
class ThreadWrapper {
public:
	explicit ThreadWrapper(T* obj) : object(obj) {}

	void operator () (typename T::ThreadType a) {
		object->startThread(a);
	}

private:
	T* object;
};

void startServer() {
	Tuntap veth;
	veth.start(wrapper::inet_addr_("10.20.30.1"),
			wrapper::inet_addr_("255.255.255.0"));
	DatagramSocket sock;
	sock.bind(config.local);

	ThreadWrapper<TunnelServer>
			server(new TunnelServer(&sock, &veth, config.secret.c_str()));
	std::thread a(server, TunnelServer::ThreadType::kNetworkDatagram);
	std::thread b(server, TunnelServer::ThreadType::kVirtualEthernet);
	std::thread c(server, TunnelServer::ThreadType::kRemoveZombie);
	a.join();
	b.join();
	c.join();
}

bool startSession(Tuntap* veth, DatagramSocket* sock,
		const sockaddr_in& server, SessionInfo* info) {
	struct [[gnu::packed]] {
		PacketHeader header;
		StartSession payload;
	} request;
	request.header.length = sizeof(request.payload);
	request.header.identity = 0;
	request.header.type = PacketHeader::Type::kStartSession;
	veth->getAddress(request.payload.mac.addr);

	DummyCrypto crypto("jiubugaosuni");
	crypto.encrypt(&request, sizeof(request));

	struct [[gnu::packed]] {
		PacketHeader header;
		SessionInfo info;
	} response;
	for (int i = 0; i < 3; ++i) {
		sock->send(server, &request, sizeof(request));
		int r = sock->recv(nullptr, &response, sizeof(response), 2000);
		if (r == sizeof(response)) {
			crypto.decrypt(&response, sizeof(response));
			std::memcpy(info, &response.info, sizeof(*info));
			return true;
		}
		MUDDY_ERROR << "recv() == " << r << ", will retry " << (2 - i)
				<< " time(s).";
	}
	return false;
}

void startClient() {
	Tuntap veth;
	DatagramSocket sock;
	SessionInfo info;
	if (!startSession(&veth, &sock, config.remote, &info)) {
		MUDDY_ERROR << "startSession() failed.\n";
		return;
	}
	veth.start(info.address, info.netmask);

	ThreadWrapper<TunnelClient> client(
			new TunnelClient(&sock, &veth, "jiubugaosuni", info.identity,
					config.remote));
	std::thread a(client, TunnelClient::ThreadType::kHeartbeat);
	std::thread b(client, TunnelClient::ThreadType::kNetworkDatagram);
	std::thread c(client, TunnelClient::ThreadType::kVirtualEthernet);
	a.join();
	b.join();
	c.join();
}

} // namespace

int main(int argc, char** argv) try {
	if (!parseCommandLine(argc, argv)) {
		return 1;
	}
	if (config.local.sin_port != 0) {
		startServer();
	} else {
		startClient();
	}
	return 0;
} catch (IgnoredExcept& e) {
	std::cerr << e.what() << "\n";
	return 1;
}
