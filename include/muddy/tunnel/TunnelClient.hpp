#ifndef MUDDY_TUNNEL_TUNNEL_CLIENT_HPP_INCLUDED_
#define MUDDY_TUNNEL_TUNNEL_CLIENT_HPP_INCLUDED_

#include <muddy/common/util/net.hpp>
#include <muddy/net/DatagramSocket.hpp>
#include <muddy/veth/Tuntap.hpp>
#include <muddy/crypto/DummyCrypto.hpp>

namespace muddy {

class TunnelClient {
public:
	enum class ThreadType {
		kNetworkDatagram, kVirtualEthernet, kHeartbeat
	};

	TunnelClient(DatagramSocket* sock, Tuntap* veth, const char* secret,
			uint32_t identity, const sockaddr_in& remote)
			: server(remote), identity(identity), crypto(secret),
			sock(sock), veth(veth), stop(false) {}

	void startThread(ThreadType type);

private:
	sockaddr_in server;
	uint32_t identity;
	DummyCrypto crypto;
	DatagramSocket* sock;
	Tuntap* veth;
	volatile bool stop;

	void sendHeartbeat();
	void handleNetworkDatagram();
	void handleVirtualEthernet();
	bool validDataSource(const sockaddr_in& peer) const;
};

} // namespace muddy

#endif // MUDDY_TUNNEL_TUNNEL_CLIENT_HPP_INCLUDED_
