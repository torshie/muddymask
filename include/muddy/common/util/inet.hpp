#ifndef MUDDY_COMMON_UTIL_INET_HPP_INCLUDED_
#define MUDDY_COMMON_UTIL_INET_HPP_INCLUDED_

#include <muddy/common/wrapper/wrapper.hpp>

namespace muddy { namespace util {

bool isInetAddress(const char* str);
sockaddr_in getInetAddress(const char* str);

}} // namespace muddy::util

#endif // MUDDY_COMMON_UTIL_INET_HPP_INCLUDED_
