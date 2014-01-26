#include <muddy/net/DatagramSocket.hpp>

using namespace muddy::net;

void DatagramSocket::bind(const sockaddr_in& address) {
	wrapper::bind_(handle, &address, sizeof(address));
}
