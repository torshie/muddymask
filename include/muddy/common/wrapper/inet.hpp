#ifndef MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_
#define MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_

#ifndef MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <muddy/common/compat/stdint.h>
#if !HAVE_WINSOCK2_H
#include <muddy/common/wrapper/inet_posix.hpp>
#else
#include <muddy/common/wrapper/inet_win32.hpp>
#endif // #if !HAVE_WINSOCK

namespace muddy { namespace wrapper {

inline in_addr_t inet_addr_(const char* cp) {
	in_addr_t n = inet_addr(cp);
	if (n == INADDR_NONE) {
		throw InvalidInetAddress(cp);
	}
	return n;
}

}} // namespace muddy::wrapper

#endif // MUDDY_COMMON_WRAPPER_INET_HPP_INCLUDED_
