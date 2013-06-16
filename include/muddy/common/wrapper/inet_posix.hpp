#ifndef MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_
#error "Donnot include me"
#endif

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef int Socket;

namespace muddy { namespace wrapper {

inline Socket socket_(int af, int dm, int type) {
	CHECK_NON_NEGATIVE(int, socket(af, dm, type));
}

inline void bind_(int s, const sockaddr_in* addr, socklen_t len) {
	CHECK_RETURN_ZERO(bind(s, reinterpret_cast<const sockaddr*>(addr),
			len));
}

inline ssize_t sendto_(int s, const void* buf, size_t size, int flags,
		const sockaddr_in* dest, socklen_t len) {
	CHECK_NON_NEGATIVE(ssize_t, sendto(s, buf, size, flags,
			reinterpret_cast<const sockaddr*>(dest), len));
}

inline ssize_t recvfrom_(int s, void* buf, size_t size, int flags,
		sockaddr_in* src, socklen_t* len) {
	CHECK_NON_NEGATIVE(ssize_t, recvfrom(s, buf, size, flags,
			reinterpret_cast<sockaddr*>(src), len));
}

inline void closesocket_(int s) {
	CHECK_RETURN_ZERO(close(s));
}

}} // namespace muddy::wrapper
