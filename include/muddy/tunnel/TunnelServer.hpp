#ifndef MUDDY_TUNNEL_TUNNEL_SERVER_HPP_INCLUDED_
#define MUDDY_TUNNEL_TUNNEL_SERVER_HPP_INCLUDED_

#include <cstring>
#include <mutex>
#include <stack>
#include <unordered_map>
#include <muddy/net/DatagramSocket.hpp>
#include <muddy/veth/Tuntap.hpp>
#include <muddy/tunnel/packet.hpp>
#include <muddy/common/thread/shared_mutex.hpp>
#include <muddy/crypto/DummyCrypto.hpp>

namespace std { // To specialize std::hash

template<>
struct hash<::muddy::MacAddress> {
	size_t operator () (const ::muddy::MacAddress& key) const {
		if (sizeof(size_t) == 4) {
			uint32_t a1;
			uint16_t a2;
			std::memcpy(&a1, key.addr, sizeof(a1));
			std::memcpy(&a2, key.addr + sizeof(a1), sizeof(a2));
			return a1 + a2;
		} else if (sizeof(size_t) == 8) {
			size_t r = 0;
			std::memcpy(&r, key.addr, 6);
			return r;
		} else {
			throw ::muddy::LostOurWay();
		}
	}
};

template<>
struct hash<::muddy::InetAddress> {
	size_t operator () (const ::muddy::InetAddress& key) const {
		if (sizeof(size_t) == 4) {
			return key.addr + key.port;
		} else if (sizeof(size_t) == 8) {
			return ((size_t)key.addr << 16 | key.port);
		} else {
			throw ::muddy::LostOurWay();
		}
	}
};

} // namespace std

namespace muddy {

class TunnelServer {
public:
	enum class ThreadType {
		kNetworkDatagram, kVirtualEthernet, kRemoveZombie
	};

	TunnelServer(DatagramSocket* sock, Tuntap* veth, const char* secret)
			: crypto(secret), sock(sock), veth(veth) {
		config.gateway = wrapper::inet_addr_("10.20.30.1");
		config.netmask = wrapper::inet_addr_("255.255.255.0");
		std::memset(&config.vmac, 0, sizeof(config.vmac));
		veth->getAddress(config.vmac.addr);
		pool.current = ntohl(wrapper::inet_addr_("10.20.30.100"));
		pool.upper = ntohl(wrapper::inet_addr_("10.20.30.200"));
	}

	void startThread(ThreadType type);

private:
	struct ClientInfo {
		struct {
			InetAddress inet;
			MacAddress mac;
			in_addr_t vip;
		} address;
		time_t lastActivity;
	};

	struct AddressPool {
		uint32_t upper;
		uint32_t current;
		std::stack<in_addr_t> free;
		std::mutex mutex;
	} pool;

	struct {
		in_addr_t gateway;
		in_addr_t netmask;
		MacAddress vmac;
	} config;

	struct {
		std::unordered_map<InetAddress, ClientInfo*> inet;
		std::unordered_map<MacAddress, ClientInfo*> mac;
		shared_mutex shared;
	} dict;

	DummyCrypto crypto;
	DatagramSocket* sock;
	Tuntap* veth;

	void handleNetworkDatagram();
	void handleVirtualEthernet();
	void removeZombieClient();
	void broadcastEthernetFrame(EthernetHeader* header, int size);
	void unicastEthernetFrame(EthernetHeader* header, int size);
	void relayData(const sockaddr_in& peer, const char* packet,
			int length);
	void startSession(const sockaddr_in& peer, const char* packet,
			int length);
	void closeSession(const sockaddr_in& peer, const char* packet,
			int length);
	void handleHeartbeat(const sockaddr_in& peer, const char* packet,
			int length);
	void handleDuplicatedSession(ClientInfo* info, const char* packet);
	in_addr_t allocateAddress();
	void releaseAddress(in_addr_t address);
	void sendSessionInfo(const sockaddr_in& peer, in_addr_t vip);
};

} // namespace muddy

#endif // MUDDY_TUNNEL_TUNNEL_SERVER_HPP_INCLUDED_
