#ifndef MUDDY_COMPAT_STDINT_H_INCLUDED_
#define MUDDY_COMPAT_STDINT_H_INCLUDED_

#include <config.h>

#if HAVE_STDINT_H
#	include <stdint.h>
#else

namespace muddy {

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

} // namespace muddy

#endif

#endif // MUDDY_COMPAT_STDINT_H_INCLUDED_
