#include <muddy/common/wrapper.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/net/DatagramSocket.hpp>

using namespace muddy;

namespace {

void ioctlsocket_(SocketHandle sock, long cmd, u_long* arg) {
	if (ioctlsocket(sock, cmd, arg) != 0) {
		throw SystemError{WSAGetLastError(), " ioctlsocket() "};
	}
}

} // namespace

int DatagramSocket::timedRecv(sockaddr_in* address, void* data,
		int size, int millisecond) {
	struct CleanUp {
		SocketHandle h;

		CleanUp(SocketHandle h) : h(h) {
			u_long enable = 1;
			ioctlsocket_(h, FIONBIO, &enable);
		}

		~CleanUp() {
			u_long disable = 0;
			ioctlsocket_(h, FIONBIO, &disable);
		}
	} cleanup{handle};

	fd_set set;
	FD_ZERO(&set);
	FD_SET(handle, &set);
	timeval tv;
	tv.tv_sec = millisecond / 1000;
	tv.tv_usec = (millisecond % 1000) * 1000;
	int r = select(0, &set, nullptr, nullptr, &tv);
	if (r == 0) {
		return 0;
	} else if (r == SOCKET_ERROR) {
		return -1;
	}
	socklen_t l = sizeof(*address);
	int n = recvfrom(handle, static_cast<char*>(data), size, 0,
			reinterpret_cast<sockaddr*>(address),
			address == nullptr ? nullptr : &l);
	if (n == SOCKET_ERROR) {
		auto e = WSAGetLastError();
		if (e == WSAEWOULDBLOCK) {
			return 0;
		}
		throw SystemError{e, " recvfrom()"};
	}
	return n;
}
