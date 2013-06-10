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

#include <muddy/common/wrapper/libc.hpp>

#undef CHECK_RETURN_ZERO
#undef CHECK_NON_NEGATIVE

#endif // MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
