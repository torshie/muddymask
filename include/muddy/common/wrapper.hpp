#ifndef MUDDY_COMMON_WRAPPER_HPP_
#define MUDDY_COMMON_WRAPPER_HPP_

#include <muddy/config.h>
#include <cerrno>
#include <cstdio>

#define MUDDY_ASSURE_ZERO(...) \
	::muddy::inner::wrapper::assureZero(__VA_ARGS__)
#if HAVE_SYSCALL_UNIX
#	define MUDDY_ASSURE_NATURAL(...) \
		return ::muddy::inner::wrapper::assureNatural(__VA_ARGS__)
#elif HAVE_SYSCALL_WIN32 // #if HAVE_SYSCALL_UNIX
#	define MUDDY_ASSURE_TRUE(...) \
		::muddy::inner::wrapper::assureTrue(__VA_ARGS__)
#else // #elif HAVE_SYSCALL_WIN32
#	error "Your platform isn't supported."
#endif // #if HAVE_SYSCALL_UNIX

#include <muddy/common/util/define.hpp>
#include <muddy/common/except.hpp>
#include <muddy/support/net.hpp>
#include <muddy/common/wrapper/inner.hpp>
#include <muddy/common/wrapper/socket.hpp>
#include <muddy/common/wrapper/libc.hpp>

#if HAVE_SYSCALL_UNIX
#	include <muddy/common/wrapper/unix.hpp>
#	include <muddy/common/wrapper/socket_unix.hpp>
#elif HAVE_SYSCALL_WIN32 // #if HAVE_SYSCALL_UNIX
#	include <muddy/common/wrapper/windows.hpp>
#	include <muddy/common/wrapper/imagehlp.hpp>
#	include <muddy/common/wrapper/socket_win32.hpp>
#else // #elif HAVE_SYSCALL_WIN32
#	error "Your platform isn't supported."
#endif // #if HAVE_SYSCALL_UNIX

#ifdef MUDDY_ASSURE_ZERO
#	undef MUDDY_ASSURE_ZERO
#endif
#ifdef MUDDY_ASSURE_NATURAL
#	undef MUDDY_ASSURE_NATURAL
#endif
#ifdef MUDDY_ASSURE_TRUE
#	undef MUDDY_ASSURE_TRUE
#endif

#endif // MUDDY_COMMON_WRAPPER_HPP_
