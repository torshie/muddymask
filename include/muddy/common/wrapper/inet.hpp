#ifndef MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_
#define MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_

#ifndef MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <muddy/common/compat/stdint.h>
#if !_WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#else
// On windows it's a must to include <winsock2.h> before <windows.h>,
// so <winsock2.h> cannot be included here.
#endif

#if !HAVE_IN_ADDR_T
typedef uint32_t in_addr_t;
#endif

namespace muddy { namespace wrapper {

inline in_addr_t inet_addr_(const char* cp) {
	in_addr_t n = inet_addr(cp);
	if (n == INADDR_NONE || n == INADDR_ANY) {
		throw InvalidInetAddress(cp);
	}
	return n;
}

}} // namespace muddy::wrapper

#endif // MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_
