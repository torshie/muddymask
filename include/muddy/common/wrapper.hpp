#ifndef MUDDY_COMMON_WRAPPER_HPP_
#define MUDDY_COMMON_WRAPPER_HPP_

#if _WIN32
#	include <muddy/common/wrapper/wrapper_win32.hpp>
#else
#	include <muddy/common/wrapper/wrapper_unix.hpp>
#endif

#endif // MUDDY_COMMON_WRAPPER_HPP_
