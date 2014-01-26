#ifndef MUDDY_COMMON_UTIL_DEFINE_HPP_
#define MUDDY_COMMON_UTIL_DEFINE_HPP_

#define MUDDY_UNLIKELY(x) __builtin_expect((x), false)
#define MUDDY_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define MUDDY_ARENA_ALLOC(Type) \
	static void* operator new (size_t size) { \
		MUDDY_ASSERT(size == sizeof(Type)); \
		enum { \
			kSize = sizeof(void*) > sizeof(Type) \
					? sizeof(void*) : sizeof(Type), \
			kAlign = std::alignment_of<Type>::value \
		}; \
		using Alloc = ::muddy::ArenaAllocator<kSize, kAlign>; \
		return Alloc::single().allocate(); \
	} \
	static void operator delete(void* p) { \
		enum { \
			kSize = sizeof(void*) > sizeof(Type) \
					? sizeof(void*) : sizeof(Type), \
			kAlign = std::alignment_of<Type>::value \
		}; \
		using Alloc = ::muddy::ArenaAllocator<kSize, kAlign>; \
		Alloc::single().release(p); \
	}

#if MUDDY_ENABLE_DEBUGGING
#	define MUDDY_ASSERT(x, ...) do { \
		if (!(x)) { \
			throw WrongAssertion(#x, ##__VA_ARGS__); \
		} \
	} while (false)
#else
#	define MUDDY_ASSERT(x, ...) (void)0
#endif

#endif // MUDDY_COMMON_UTIL_DEFINE_HPP_
