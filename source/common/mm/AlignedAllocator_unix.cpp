#include <muddy/common/tpl/align.hpp>
#include <muddy/common/util/define.hpp>
#include <muddy/common/wrapper.hpp>
#include <muddy/common/mm/AlignedAllocator.hpp>

using namespace muddy;

void* AlignedAllocator::allocate(size_t size) {
	MUDDY_ASSERT(size % kPageSize == 0 && size > 0);
	void* pointer = wrapper::mmap_(nullptr, size + alignment, PROT_NONE,
					MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	uintptr_t base = reinterpret_cast<uintptr_t>(pointer);
	uintptr_t aligned = base % alignment * alignment + alignment;
	wrapper::munmap_(pointer, aligned - base);
	if (aligned - base < alignment) {
		wrapper::munmap_(reinterpret_cast<void*>(aligned + size),
				alignment - (aligned - base));
	}
	pointer = reinterpret_cast<void*>(aligned);
	wrapper::mmap_(pointer, size, PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	return pointer;
}

void AlignedAllocator::release(void* p, size_t size) {
	MUDDY_ASSERT(size % kPageSize == 0 && size > 0);
	wrapper::munmap_(p, size);
}
