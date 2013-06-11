#include <winsock2.h>
#include <muddy/common/except.hpp>
#include <muddy/common/inet/DatagramSocket_win32.hpp>

#define MUDDY_DECLARE_WINSOCK_INIT_HELPER_
#include <muddy/common/inet/WinsockInitHelper.hpp>

WinsockInitHelper winsockInitHelper;

using namespace muddy;

DatagramSocket::DatagramSocket() {
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
		throw Unpossible();
	}
	(void)winsockInitHelper;
}

DatagramSocket::~DatagramSocket() {
	if (closesocket(sock) != 0) {
		throw Unpossible();
	}
}

void DatagramSocket::bind(const sockaddr_in& address) {
	if (::bind(sock, (const sockaddr*)&address, sizeof(address)) != 0) {
		throw Unpossible();
	}
}

int DatagramSocket::read(void* buffer, int size, sockaddr_in* client) {
	int len = 0;
	if (client != NULL) {
		len = sizeof(*client);
	}
	int n = recvfrom(sock, static_cast<char*>(buffer), size, 0,
			reinterpret_cast<sockaddr*>(client), &len);
	if (n == SOCKET_ERROR) {
		throw Unpossible();
	}
	return n;
}

int DatagramSocket::write(const void* buffer, int size,
		const sockaddr_in& remote) {
	int n = sendto(sock, static_cast<const char*>(buffer), size, 0,
			reinterpret_cast<const sockaddr*>(&remote), sizeof(remote));
	if (n == SOCKET_ERROR) {
		throw Unpossible();
	}
	return n;
}
