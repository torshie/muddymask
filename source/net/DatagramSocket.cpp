#include <muddy/net/DatagramSocket.hpp>

using namespace muddy;

void DatagramSocket::bind(const sockaddr_in& address) {
	wrapper::bind_(handle, &address, sizeof(address));
}

int DatagramSocket::send(const sockaddr_in& address, const void* data,
		int size) {
	return wrapper::sendto_(handle, data, size, 0, &address,
			sizeof(address));
}

int DatagramSocket::recv(sockaddr_in* address, void* data, int size,
		int millisecond) {
	socklen_t l = sizeof(*address);
	if (millisecond < 0) {
		return wrapper::recvfrom_(handle, data, size, 0, address, &l);
	}
	return timedRecv(address, data, size, millisecond);
}
