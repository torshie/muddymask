#ifndef MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#define MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_

#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>

#define CHECK_RETURN_ZERO(...) \
	if (UNLIKELY((__VA_ARGS__) != 0)) { \
		throw SystemError(errno); \
	} \
	(void)0

#define CHECK_NON_NEGATIVE(type, ...) \
	type value = (__VA_ARGS__); \
	if (UNLIKELY(value < 0)) { \
		throw SystemError(errno); \
	} \
	return value

#if _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include <muddy/common/wrapper/libc.hpp>
#include <muddy/common/wrapper/inet.hpp>
#if !_WIN32
#include <muddy/common/wrapper/unix.hpp>
#endif

#undef CHECK_RETURN_ZERO
#undef CHECK_NON_NEGATIVE

#endif // MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
