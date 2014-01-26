#ifndef MUDDY_NET_CLOSE_SOCKET_HANDLE_HPP_
#define MUDDY_NET_CLOSE_SOCKET_HANDLE_HPP_

#if HAVE_MUDDY_CONFIG_H
#	include <muddy/config.h>
#endif

#include <muddy/support/net.hpp>

namespace muddy { namespace net {

inline void closeSocketHandle(SocketHandle handle) {
#if HAVE_SOCK_UNIX
	if (MUDDY_UNLIKELY(close(handle) != 0)) {
		throw SystemError{errno};
	}
#elif HAVE_SOCK_WINSOCK2
	if (MUDDY_UNLIKELY(closesocket(handle) != 0)) {
		throw SystemError{WSAGetLastError()};
	}
#else
#	error "Your platform isn't supported."
#endif
}

}} // namespace muddy::net

#endif // MUDDY_NET_CLOSE_SOCKET_HANDLE_HPP_
