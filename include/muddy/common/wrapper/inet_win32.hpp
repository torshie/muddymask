#ifndef MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_
#error "Donnot #include me"
#endif

#include <winsock2.h>
#include <windows.h>

typedef uint32_t in_addr_t;
typedef SOCKET Socket;
typedef int socklen_t;
typedef long ssize_t;

namespace muddy { namespace wrapper {

inline Socket socket_(int af, int dm, int tp) {
	Socket s = socket(af, dm, tp);
	if (s == INVALID_SOCKET) {
		throw Unpossible();
	}
	return s;
}

inline void bind_(Socket s, const sockaddr_in* address, socklen_t len) {
	if (bind(s, reinterpret_cast<const sockaddr*>(address), len) != 0) {
		throw Unpossible();
	}
}

inline ssize_t sendto_(Socket s, const void* buf, size_t size, int flags,
		const sockaddr_in* dest, socklen_t len) {
	ssize_t r = sendto(s, static_cast<const char*>(buf), size, flags,
			reinterpret_cast<const sockaddr*>(dest), len);
	if (r == SOCKET_ERROR) {
		throw Unpossible();
	}
	return r;
}

inline ssize_t recvfrom_(Socket s, void* buf, size_t size, int flags,
		sockaddr_in* src, socklen_t* len) {
	ssize_t r = recvfrom(s, static_cast<char*>(buf), size, flags,
			reinterpret_cast<sockaddr*>(src), len);
	if (r == SOCKET_ERROR) {
		throw Unpossible();
	}
	return r;
}

inline void closesocket_(Socket s) {
	if (closesocket(s) != 0) {
		throw Unpossible();
	}
}

}} // namespace muddy::wrapper
