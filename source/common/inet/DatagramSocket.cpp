#if _WIN32
#include <winsock2.h>
#endif
#include <muddy/common/except.hpp>
#include <muddy/common/inet/DatagramSocket.hpp>

#if _WIN32
#define MUDDY_DECLARE_WINSOCK_INIT_HELPER_
#include <muddy/common/inet/WinsockInitHelper.hpp>
WinsockInitHelper winsockInitHelper;
#endif

using namespace muddy;

DatagramSocket::DatagramSocket() {
	sock = wrapper::socket_(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#if _WIN32
	(void)winsockInitHelper;
#endif
}

DatagramSocket::~DatagramSocket() {
	wrapper::closesocket_(sock);
}

void DatagramSocket::bind(const sockaddr_in& address) {
	wrapper::bind_(sock, &address, sizeof(address));
}

int DatagramSocket::read(void* buffer, int size, sockaddr_in* client) {
	socklen_t len = sizeof(*client);
	return wrapper::recvfrom_(sock, static_cast<char*>(buffer), size, 0,
			client, &len);
}

int DatagramSocket::write(const void* buffer, int size,
		const sockaddr_in& remote) {
	return wrapper::sendto_(sock, static_cast<const char*>(buffer), size,
			0, &remote, sizeof(remote));
}
