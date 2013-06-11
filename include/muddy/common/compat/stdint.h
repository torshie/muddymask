#ifndef MUDDY_COMMON_COMPAT_STDINT_H_INCLUDED_
#define MUDDY_COMMON_COMPAT_STDINT_H_INCLUDED_

#include <config.h>

#if HAVE_STDINT_H
#	include <stdint.h>
#else

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef typename muddy::helper::FirstType<
	sizeof(void*) == sizeof(int),
	int,
	muddy::helper::FirstType<
		sizeof(void*) == sizeof(long),
		long,
		muddy::helper::FirstType<
			sizeof(void*) == sizeof(long long),
			long long,
			void
		>::T
	>::T
>::T intptr_t;

typedef typename muddy::helper::FirstType<
	sizeof(void*) == sizeof(int),
	unsigned,
	muddy::helper::FirstType<
		sizeof(void*) == sizeof(long),
		unsigned long,
		muddy::helper::FirstType<
			sizeof(void*) == sizeof(long long),
			unsigned long long,
			void
		>::T
	>::T
>::T uintptr_t;

#endif // #if HAVE_STDINT_H

#endif // MUDDY_COMMON_COMPAT_STDINT_H_INCLUDED_
