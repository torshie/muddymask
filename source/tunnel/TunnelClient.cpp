#include <ctime>
#include <cstring>
#include <muddy/support/net.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/util/misc.hpp>
#include <muddy/tunnel/TunnelClient.hpp>
#include <muddy/tunnel/packet.hpp>

using namespace muddy;

void TunnelClient::startThread(ThreadType type) {
	switch (type) {
	case ThreadType::kHeartbeat:
		sendHeartbeat();
		break;
	case ThreadType::kNetworkDatagram:
		logging::Logger::get().setIdentity("net");
		handleNetworkDatagram();
		break;
	case ThreadType::kVirtualEthernet:
		logging::Logger::get().setIdentity("veth");
		handleVirtualEthernet();
		break;
	default:
		throw LostOurWay();
	}
}

void TunnelClient::sendHeartbeat() {
	struct [[gnu::packed]] {
		PacketHeader header;
		Heartbeat heartbeat;
	} data;
	data.header._zero = 0;
	data.header.length = sizeof(data.heartbeat);
	data.header.type = PacketHeader::Type::kHeartbeat;
	data.heartbeat.i = std::time(nullptr);

	crypto.encrypt(&data, sizeof(data));
	while (true) {
		util::sleep(15000);
		sock->send(server, &data, sizeof(data));
	}
}

void TunnelClient::handleNetworkDatagram() {
	while (true) {
		char packet[2000];
		sockaddr_in peer;
		size_t size = sock->recv(&peer, packet, sizeof(packet));
		if (size < sizeof(PacketHeader) || !validDataSource(peer)) {
			MUDDY_DEBUG << "invalid packet received";
			continue;
		}
		crypto.decrypt(packet, size);
		auto header = reinterpret_cast<PacketHeader*>(packet);
		if (header->type != PacketHeader::Type::kRelayData
				|| header->length < sizeof(EthernetHeader)
				|| header->length + sizeof(PacketHeader) != size) {
			MUDDY_DEBUG << "packet size incorrect";
			continue;
		}
		veth->write(header + 1, header->length);
	}
}

void TunnelClient::handleVirtualEthernet() {
	struct [[gnu::packed]] {
		PacketHeader header;
		unsigned char data[2000];
	} packet;
	packet.header._zero = 0;
	packet.header.type = PacketHeader::Type::kRelayData;

	while (true) {
		char buffer[sizeof(packet)];
		ssize_t s = veth->read(packet.data, sizeof(packet.data));
		packet.header.length = s;
		std::memcpy(buffer, &packet, s + sizeof(packet.header));
		crypto.encrypt(buffer, s + sizeof(packet.header));
		sock->send(server, &buffer, s + sizeof(packet.header));
	}
}

bool TunnelClient::validDataSource(const sockaddr_in& peer) const {
	return peer.sin_family == AF_INET
			&& peer.sin_addr.s_addr == server.sin_addr.s_addr
			&& peer.sin_port == server.sin_port;
}
