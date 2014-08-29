#include <ctime>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/util/misc.hpp>
#include <muddy/common/tpl/shared_lock.hpp>
#include <muddy/tunnel/TunnelServer.hpp>

using namespace muddy;

void TunnelServer::startThread(ThreadType type) {
	switch (type) {
	case ThreadType::kNetworkDatagram:
		logging::Logger::get().setIdentity("net");
		handleNetworkDatagram();
		break;
	case ThreadType::kVirtualEthernet:
		logging::Logger::get().setIdentity("veth");
		handleVirtualEthernet();
		break;
	case ThreadType::kRemoveZombie:
		logging::Logger::get().setIdentity("poll");
		removeZombieClient();
		break;
	default:
		throw LostOurWay();
	}
}

void TunnelServer::handleNetworkDatagram() {
	while (true) {
		char packet[2000];
		sockaddr_in peer;
		ssize_t size = sock->recv(&peer, packet, sizeof(packet));
		if (size < (ssize_t)sizeof(PacketHeader) || size > 1600) {
			MUDDY_DEBUG << "Unexpected UDP packet, ignored.";
			continue;
		}
		crypto.decrypt(packet, size);

		auto header = reinterpret_cast<PacketHeader*>(packet);
		if (header->length + (ssize_t)sizeof(PacketHeader) != size) {
			MUDDY_DEBUG << "Invalid packet size, ignored.";
			continue;
		}

		typedef void (TunnelServer::*Handler)(const sockaddr_in&,
				const char*, int);
		static const Handler kHandlerTable[] = {
				nullptr, &TunnelServer::handleHeartbeat,
				&TunnelServer::startSession, nullptr,
				&TunnelServer::closeSession, &TunnelServer::relayData
		};

		if (header->type > 0
				&& header->type < PacketHeader::Type::kTypeCount) {
			Handler h = kHandlerTable[header->type];
			if (h != nullptr) {
				(this->*h)(peer, packet + sizeof(PacketHeader),
						header->length);
			}
		}
	}
}

void TunnelServer::handleVirtualEthernet() {
	while (true) {
		char frame[2000];
		ssize_t size = veth->read(frame, sizeof(frame));
		if (size < (ssize_t)sizeof(EthernetHeader)) {
			MUDDY_DEBUG << "data read from virtual interface is invalid";
			continue;
		}
		auto ether = reinterpret_cast<EthernetHeader*>(frame);
		if (ether->dst.addr[0] % 2 == 1) {
			broadcastEthernetFrame(ether, size);
		} else {
			unicastEthernetFrame(ether, size);
		}
	}
}

void TunnelServer::removeZombieClient() {
	while (true) {
		util::sleep(15000);
		MUDDY_DEBUG << "Trying to lock server for zombie removing";
		std::lock_guard<shared_mutex> guard(dict.shared);
		MUDDY_DEBUG << "Got mutex, start working";
		time_t now = std::time(nullptr);
		for (auto it = dict.inet.begin(); it != dict.inet.end(); ) {
			if (now - it->second->lastActivity < 45) {
				++it;
				continue;
			}
			auto info = it->second;
			MUDDY_DEBUG << "Removing client with VIP: " << std::hex
					<< info->address.vip;
			dict.mac.erase(info->address.mac);
			auto tmp = it;
			++it;
			dict.inet.erase(tmp);
			releaseAddress(info->address.vip);
			delete info;
		}
	}
}

void TunnelServer::broadcastEthernetFrame(EthernetHeader* header,
		int size) {
	PacketHeader h;
	h._zero = 0;
	h.length = size;
	h.type = PacketHeader::Type::kRelayData;
	char final[sizeof(h) + size];
	std::memcpy(final, &h, sizeof(h));
	std::memcpy(final + sizeof(h), header, size);
	crypto.encrypt(final, sizeof(h) + size);

	sockaddr_in dst;
	std::memset(&dst, 0, sizeof(dst));
	dst.sin_family = AF_INET;

	tpl::shared_lock<shared_mutex> lock(dict.shared);
	for (const auto& info : dict.mac) {
		dst.sin_addr.s_addr = info.second->address.inet.addr;
		dst.sin_port = info.second->address.inet.port;
		sock->send(dst, final, size + sizeof(h));
	}
}

void TunnelServer::unicastEthernetFrame(EthernetHeader* header, int size) {
	sockaddr_in dst;
	std::memset(&dst, 0, sizeof(dst));
	dst.sin_family = AF_INET;
	{
		tpl::shared_lock<shared_mutex> lock(dict.shared);
		auto it = dict.mac.find(header->dst);
		if (it == dict.mac.end()) {
			MUDDY_DEBUG << "Unicast destination invalid, packet dropped";
			return;
		}
		dst.sin_addr.s_addr = it->second->address.inet.addr;
		dst.sin_port = it->second->address.inet.port;
	}

	PacketHeader h;
	h._zero = 0;
	h.length = size;
	h.type = PacketHeader::Type::kRelayData;

	char final[sizeof(h) + size];
	std::memcpy(final, &h, sizeof(h));
	std::memcpy(final + sizeof(h), header, size);
	crypto.encrypt(final, sizeof(h) + size);
	sock->send(dst, final, sizeof(h) + size);
}

void TunnelServer::relayData(const sockaddr_in& peer, const char* packet,
		int length) {
	if (length <= (ssize_t)sizeof(EthernetHeader)) {
		MUDDY_DEBUG << "invalid relay data packet";
		return;
	}
	{
		tpl::shared_lock<shared_mutex> lock{dict.shared};
		if (dict.inet.find(peer) == dict.inet.end()) {
			MUDDY_DEBUG << "Ignore data packet from unknown client.";
			return;
		}
	}
	auto header = reinterpret_cast<const EthernetHeader*>(packet);
	if (header->dst.addr[0] % 2 != 1 && header->dst != config.vmac) {
		MUDDY_DEBUG << "Unknown frame destination";
		return;
	}
	veth->write(packet, length);
}

void TunnelServer::startSession(const sockaddr_in& peer,
		const char* packet, int length) {
	if (length != sizeof(StartSession)) {
		MUDDY_DEBUG << "invalid StartSession packet";
		return;
	}
	{
		tpl::shared_lock<shared_mutex> lock{dict.shared};
		auto it = dict.inet.find(peer);
		if (it != dict.inet.end()) {
			handleDuplicatedSession(it->second, packet);
			return;
		}
	}

	in_addr_t vip = allocateAddress();
	if (vip == 0) {
		MUDDY_DEBUG << "cannot allocate address";
		return;
	}
	MUDDY_INFO << "Allocated address for client: " << std::hex << vip
			<< std::dec;

	auto info = new ClientInfo();
	info->address.vip = vip;
	info->address.inet = peer;
	info->address.mac = reinterpret_cast<const StartSession*>(packet)->mac;
	info->lastActivity = std::time(nullptr);

	{
		std::lock_guard<shared_mutex> guard(dict.shared);
		dict.inet.insert(std::make_pair(info->address.inet, info));
		dict.mac.insert(std::make_pair(info->address.mac, info));
	}

	sendSessionInfo(peer, vip);
}

void TunnelServer::closeSession(const sockaddr_in& peer, const char*,
		int length) {
	if (length != 0) {
		MUDDY_DEBUG << "invalid CloseSession packet";
		return;
	}
	{
		tpl::shared_lock<shared_mutex> lock(dict.shared);
		if (dict.inet.find(peer) == dict.inet.end()) {
			return;
		}
	}

	in_addr_t vip;
	{
		std::lock_guard<shared_mutex> guard(dict.shared);
		auto it = dict.inet.find(peer);
		if (it == dict.inet.end()) {
			return;
		}
		auto info = it->second;
		vip = info->address.vip;
		dict.inet.erase(info->address.inet);
		dict.mac.erase(info->address.mac);
		delete info;
	}

	releaseAddress(vip);
}

void TunnelServer::handleHeartbeat(const sockaddr_in& peer,
		const char* /* packet */, int length) {
	if (length != sizeof(Heartbeat)) {
		MUDDY_DEBUG << "invalid Heartbeat packet";
		return;
	}

	tpl::shared_lock<shared_mutex> lock{dict.shared};
	auto it = dict.inet.find(peer);
	if (it == dict.inet.end()) {
		MUDDY_DEBUG << "Ignore heartbeat from unknown client.";
		return;
	}
	it->second->lastActivity = std::time(nullptr);
}

void TunnelServer::handleDuplicatedSession(ClientInfo* info,
		const char* packet) {
	if (std::memcmp(packet, info->address.mac.addr, 6) != 0) {
		MUDDY_ERROR << "Source mac address changed, ignore start session "
				"request";
		return;
	}
	MUDDY_DEBUG << "StartSession duplicated, re-send old info.";
	sendSessionInfo(info->address.inet, info->address.vip);
}

in_addr_t TunnelServer::allocateAddress() {
	std::lock_guard<std::mutex> guard(pool.mutex);

	if (pool.current < pool.upper) {
		in_addr_t r = htonl(pool.current);
		++pool.current;
		return r;
	}

	if (!pool.free.empty()) {
		in_addr_t r = pool.free.top();
		pool.free.pop();
		return r;
	}

	return 0;
}

void TunnelServer::releaseAddress(in_addr_t address) {
	std::lock_guard<std::mutex> guard(pool.mutex);
	pool.free.push(address);
}

void TunnelServer::sendSessionInfo(const sockaddr_in& peer,
		in_addr_t vip) {
	struct [[gnu::packed]] {
		PacketHeader header;
		SessionInfo info;
	} response;
	response.header.length = sizeof(response.info);
	response.header.type = PacketHeader::Type::kSessionInfo;
	response.header._zero = 0;
	response.info.address = vip;
	response.info.gateway = config.gateway;
	response.info.netmask = config.netmask;

	crypto.encrypt(&response, sizeof(response));
	sock->send(peer, &response, sizeof(response));
}
