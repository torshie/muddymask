#ifndef MUDDY_COMMON_UTIL_DEFINE_HPP_INCLUDED_
#define MUDDY_COMMON_UTIL_DEFINE_HPP_INCLUDED_

#define ASSERT_SIZE(type, size) STATIC_ASSERT(sizeof(type) == size)

#define DISABLE_COPY(type) \
	void operator = (const type&); \
	type(const type&)

#define ELEMENT_COUNT(a) (sizeof(a) / sizeof(a[0]))

#define STATIC_ASSERT(a) \
	void _static_assert_failed_(::search::util::StaticAssert<(a)>)

#if ENABLE_ADVANCED_MM
#define USE_SLICE_ALLOCATOR(type) \
	static void* operator new (size_t s) { \
		typedef ::search::SliceAllocator<sizeof(type)> Allocator; \
		return Allocator::singleton().allocate(s); \
	} \
	static void operator delete (void* p) { \
		typedef ::search::SliceAllocator<sizeof(type)> Allocator; \
		Allocator::singleton().release(p); \
	}
#else
#	define USE_SLICE_ALLOCATOR(a)
#endif

#if _MSC_VER
#	define UNLIKELY(x) (x)
#else
#	define UNLIKELY(x) __builtin_expect((x), false)
#endif

#endif // MUDDY_COMMON_UTIL_DEFINE_HPP_INCLUDED_
