#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <muddy/net/DatagramSocket.hpp>

using namespace muddy;

int DatagramSocket::timedRecv(sockaddr_in* address, void* data,
		int size, int millisecond) {
	fd_set set;
	FD_ZERO(&set);
	FD_SET(handle, &set);
	timeval tv;
	tv.tv_sec = millisecond / 1000;
	tv.tv_usec = (millisecond % 1000) * 1000;
	int r = select(handle + 1, &set, nullptr, nullptr, &tv);
	if (r == 0) {
		return 0;
	} else if (r < 0) {
		return -1;
	}
	socklen_t l = sizeof(*address);
	int n = recvfrom(handle, data, size, MSG_DONTWAIT,
			reinterpret_cast<sockaddr*>(address), &l);
	if (n < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return 0;
		}
		return -1;
	}
	return n;
}
