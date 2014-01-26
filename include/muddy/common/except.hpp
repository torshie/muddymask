#ifndef MUDDY_COMMON_EXCEPT_HPP_
#define MUDDY_COMMON_EXCEPT_HPP_

#include <muddy/common/except/IgnoredExcept.hpp>
#include <muddy/common/except/BadErrno.hpp>
#include <muddy/common/except/InvalidArgument.hpp>
#include <muddy/common/except/LostOurWay.hpp>
#include <muddy/common/except/Unpossible.hpp>
#include <muddy/common/except/WrongAssertion.hpp>

#if _WIN32
#	include <muddy/common/except/WinApiError.hpp>
#endif

#endif // MUDDY_COMMON_EXCEPT_HPP_
