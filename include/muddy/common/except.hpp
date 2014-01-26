#ifndef MUDDY_COMMON_EXCEPT_HPP_
#define MUDDY_COMMON_EXCEPT_HPP_

#if HAVE_MUDDY_CONFIG_H
#	include <muddy/config.h>
#endif

#include <muddy/common/except/IgnoredExcept.hpp>
#include <muddy/common/except/InvalidArgument.hpp>
#include <muddy/common/except/LostOurWay.hpp>
#include <muddy/common/except/Unpossible.hpp>
#include <muddy/common/except/WrongAssertion.hpp>

#if HAVE_SYSCALL_UNIX
#	include <muddy/common/except/SystemError_unix.hpp>
#elif HAVE_SYSCALL_WIN32
#	include <muddy/common/except/SystemError_win32.hpp>
#else
#	error "Your platform isn't supported."
#endif

#endif // MUDDY_COMMON_EXCEPT_HPP_
