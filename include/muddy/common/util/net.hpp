#ifndef MUDDY_COMMON_UTIL_NET_HPP_INCLUDED_
#define MUDDY_COMMON_UTIL_NET_HPP_INCLUDED_

#include <muddy/support/net.hpp>

namespace muddy { namespace util {

sockaddr_in getInetAddress(const char* str);

}} // namespace muddy::util

#endif // MUDDY_COMMON_UTIL_NET_HPP_INCLUDED_
