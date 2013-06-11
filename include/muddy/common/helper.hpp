#ifndef MUDDY_COMMON_HELPER_HPP_INCLUDED_
#define MUDDY_COMMON_HELPER_HPP_INCLUDED_

#include <muddy/common/compat/stdint.h>

namespace muddy { namespace helper {

template<typename T> struct SignedInteger;

#define SPECIALIZE_SIGNED_INTEGER(type, value) \
	template<> \
	struct SignedInteger<type> { \
		enum { V = value }; \
	}

SPECIALIZE_SIGNED_INTEGER(signed char, 1);
SPECIALIZE_SIGNED_INTEGER(short, 1);
SPECIALIZE_SIGNED_INTEGER(int, 1);
SPECIALIZE_SIGNED_INTEGER(long, 1);
SPECIALIZE_SIGNED_INTEGER(long long, 1);

SPECIALIZE_SIGNED_INTEGER(unsigned char, 0);
SPECIALIZE_SIGNED_INTEGER(unsigned short, 0);
SPECIALIZE_SIGNED_INTEGER(unsigned int, 0);
SPECIALIZE_SIGNED_INTEGER(unsigned long, 0);
SPECIALIZE_SIGNED_INTEGER(unsigned long long, 0);

#undef SPECIALIZE_SIGNED_INTEGER

template<typename T>
struct Negative {
	static bool check(T v) {
		return v < 0;
	}
};

#define SPECIALIZE_NEGATIVE_CHECK(type) \
	template<> \
	struct Negative<type> { \
		static bool check(type) { \
			return false; \
		} \
	}

SPECIALIZE_NEGATIVE_CHECK(unsigned char);
SPECIALIZE_NEGATIVE_CHECK(unsigned short);
SPECIALIZE_NEGATIVE_CHECK(unsigned int);
SPECIALIZE_NEGATIVE_CHECK(unsigned long);
SPECIALIZE_NEGATIVE_CHECK(unsigned long long);

#undef SPECIALIZE_NEGATIVE_CHECK

template<bool tFirst, typename A, typename B>
struct FirstType {
	typedef A T;
};
template<typename A, typename B>
struct FirstType<false, A, B> {
	typedef B T;
};

template<int tSize> struct IntegerType;

#define SPECIALIZE_INTEGER_TYPE(size) \
	template<> \
	struct IntegerType<((size) / 8)> { \
		typedef int##size##_t I; \
		typedef uint##size##_t U; \
	};

SPECIALIZE_INTEGER_TYPE(8)
SPECIALIZE_INTEGER_TYPE(16)
SPECIALIZE_INTEGER_TYPE(32)
SPECIALIZE_INTEGER_TYPE(64)

#undef SPECIALIZE_INTEGER_TYPE

}} // namespace muddy::helper

#endif // MUDDY_COMMON_HELPER_HPP_INCLUDED_
