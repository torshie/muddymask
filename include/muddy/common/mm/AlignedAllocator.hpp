#ifndef MUDDY_COMMON_ALIGNED_ALLOCATOR_HPP_
#define MUDDY_COMMON_ALIGNED_ALLOCATOR_HPP_

#include <muddy/common/util/define.hpp>

namespace muddy {

class AlignedAllocator {
public:
	explicit AlignedAllocator(size_t align) : alignment{align} {
		MUDDY_ASSERT(align % kPageSize == 0 && align > 0);
	}

	void* allocate(size_t size);
	void release(void* p, size_t size);

private:
	enum { kPageSize = 4096 };

	size_t alignment;
};

} // namespace muddy

#endif // MUDDY_COMMON_ALIGNED_ALLOCATOR_HPP_
