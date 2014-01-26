#ifndef MUDDY_NET_DATAGRAM_SOCKET_HPP_
#define MUDDY_NET_DATAGRAM_SOCKET_HPP_

#include <muddy/common/wrapper.hpp>
#include <muddy/net/closeSocketHandle.hpp>

namespace muddy { namespace net {

class DatagramSocket {
public:
	DatagramSocket() {
		handle = wrapper::socket_(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}

	~DatagramSocket() {
		closeSocketHandle(handle);
	}

	void bind(const sockaddr_in& address);
	int send(const sockaddr_in& address, const void* data, int size);
	int recv(sockaddr_in* address, void* data, int size);

private:
	SocketHandle handle;
};

}} // namespace muddy::net

#endif // MUDDY_NET_DATAGRAM_SOCKET_HPP_
