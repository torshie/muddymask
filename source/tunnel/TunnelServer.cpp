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
		if (header->version != PacketHeader::kVersion) {
			sendErrorCode(peer, ErrorCode::kWrongVersion);
			continue;
		}

		typedef void (TunnelServer::*Handler)(const sockaddr_in&,
				uint32_t, const char*, int);
		static const Handler kHandlerTable[] = {
				nullptr, &TunnelServer::handleHeartbeat,
				&TunnelServer::startSession, nullptr,
				&TunnelServer::relayData, nullptr
		};

		if (header->type == 0
				|| header->type >= PacketHeader::Type::kTypeCount
				|| kHandlerTable[header->type] == nullptr) {
			MUDDY_DEBUG << "Unknown packet type: " << header->type;
			continue;
		}
		Handler h = kHandlerTable[header->type];
		(this->*h)(peer, header->identity, packet + sizeof(PacketHeader),
				header->length);
	}
}

void TunnelServer::handleVirtualEthernet() {
	while (true) {
		char frame[2000];
		ssize_t size = veth->read(frame, sizeof(frame));
		if (size < (ssize_t)sizeof(EthernetHeader)) {
			MUDDY_DEBUG << "Data read from virtual interface is invalid";
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
		std::lock_guard<shared_mutex> guard{dict.mutex};
		time_t now = std::time(nullptr);
		for (auto it = dict.id.begin(); it != dict.id.end(); ) {
			if (now - it->second->alive < 45) {
				++it;
				continue;
			}
			auto info = it->second;
			MUDDY_DEBUG << "Removing client with VIP: " << std::hex
					<< info->tun.ip;
			dict.mac.erase(info->tun.mac);
			it = dict.id.erase(it);
			releaseAddress(info->tun.ip);
			delete info;
		}
	}
}

void TunnelServer::broadcastEthernetFrame(EthernetHeader* header,
		int size) {
	struct [[gnu::packed]] {
		PacketHeader h;
		char payload[2000];
	} data;
	data.h.identity = identity;
	data.h.length = size;
	data.h.type = PacketHeader::Type::kRelayData;
	std::memcpy(data.payload, header, size);
	crypto.encrypt(&data, sizeof(data.h) + size);

	tpl::shared_lock<shared_mutex> lock{dict.mutex};
	for (const auto& info : dict.mac) {
		tpl::shared_lock<shared_mutex> l2{info.second->mutex};
		sock->send(info.second->address, &data, size + sizeof(data.h));
	}
}

void TunnelServer::unicastEthernetFrame(EthernetHeader* header, int size) {
	struct [[gnu::packed]] {
		PacketHeader h;
		char payload[2000];
	} data;
	data.h.identity = identity;
	data.h.length = size;
	data.h.type = PacketHeader::Type::kRelayData;
	std::memcpy(data.payload, header, size);
	crypto.encrypt(&data, sizeof(data.h) + size);

	tpl::shared_lock<shared_mutex> lock{dict.mutex};
	auto it = dict.mac.find(header->dst);
	if (it == dict.mac.end()) {
		MUDDY_DEBUG << "Unicast destination invalid, packet dropped";
		return;
	}

	tpl::shared_lock<shared_mutex> l2{it->second->mutex};
	sock->send(it->second->address, &data, sizeof(data.h) + size);
}

void TunnelServer::relayData(const sockaddr_in& peer, uint32_t id,
		const char* packet, int length) {
	if (length <= (ssize_t)sizeof(EthernetHeader)) {
		MUDDY_DEBUG << "invalid relay data packet";
		return;
	}
	{
		tpl::shared_lock<shared_mutex> lock{dict.mutex};
		if (dict.id.find(id) == dict.id.end()) {
			MUDDY_DEBUG << "Data packet from unknown client.";
			sendErrorCode(peer, ErrorCode::kRestartSession);
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

void TunnelServer::startSession(const sockaddr_in& peer, uint32_t /* id */,
		const char* packet, int length) {
	if (length != sizeof(StartSession)) {
		MUDDY_DEBUG << "invalid StartSession packet";
		return;
	}

	in_addr_t vip = allocateAddress();
	if (vip == 0) {
		MUDDY_DEBUG << "Cannot allocate VIP";
		return;
	}
	MUDDY_INFO << "Allocated address for client: " << std::hex
			<< peer.sin_addr.s_addr << ", VIP: " << vip
			<< std::dec;

	auto info = new ClientInfo{};
	info->tun.ip = vip;
	info->tun.mac = reinterpret_cast<const StartSession*>(packet)->mac;
	info->address = peer;
	info->identity = random();
	info->alive = std::time(nullptr);

	std::lock_guard<shared_mutex> guard{dict.mutex};
	dict.id.insert(std::make_pair(info->identity, info));
	dict.mac.insert(std::make_pair(info->tun.mac, info));
	sendSessionInfo(peer, info->identity, vip);
}

void TunnelServer::handleHeartbeat(const sockaddr_in& peer, uint32_t id,
		const char* /* packet */, int length) {
	if (length != sizeof(Heartbeat)) {
		MUDDY_DEBUG << "invalid Heartbeat packet";
		return;
	}

	tpl::shared_lock<shared_mutex> lock{dict.mutex};
	auto it = dict.id.find(id);
	if (it == dict.id.end()) {
		MUDDY_DEBUG << "Hearbeat from unknown client.";
		sendErrorCode(peer, ErrorCode::kRestartSession);
		return;
	}
	{
		std::lock_guard<shared_mutex> guard{it->second->mutex};
		it->second->alive = std::time(nullptr);
		if (it->second->address.sin_addr.s_addr != peer.sin_addr.s_addr
				|| it->second->address.sin_port != peer.sin_port) {
			MUDDY_DEBUG << "Client address changed, "
					<< "old: " << std::hex
					<< it->second->address.sin_addr.s_addr
					<< ":" << it->second->address.sin_port
					<< ", new: " << peer.sin_addr.s_addr
					<< ":" << peer.sin_port << std::dec;
			it->second->address = peer;
		}
	}
}

in_addr_t TunnelServer::allocateAddress() {
	std::lock_guard<std::mutex> guard{pool.mutex};

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
	std::lock_guard<std::mutex> guard{pool.mutex};
	pool.free.push(address);
}

void TunnelServer::sendSessionInfo(const sockaddr_in& peer,
		uint32_t id, in_addr_t vip) {
	struct [[gnu::packed]] {
		PacketHeader header;
		SessionInfo info;
	} response;
	response.header.identity = identity;
	response.header.length = sizeof(response.info);
	response.header.type = PacketHeader::Type::kSessionInfo;
	response.info.identity = id;
	response.info.address = vip;
	response.info.gateway = config.gateway;
	response.info.netmask = config.netmask;

	crypto.encrypt(&response, sizeof(response));
	sock->send(peer, &response, sizeof(response));
}

void TunnelServer::sendErrorCode(const sockaddr_in& peer, ErrorCode code) {
	MUDDY_DEBUG << "Sending error code: " << (int)code;
	struct [[gnu::packed]] {
		PacketHeader header;
		ErrorDetail detail;
	} packet;
	packet.header.identity = identity;
	packet.header.length = sizeof(packet.detail);
	packet.header.type = PacketHeader::Type::kErrorDetail;
	packet.detail.code = code;

	crypto.encrypt(&packet, sizeof(packet));
	sock->send(peer, &packet, sizeof(packet));
}
