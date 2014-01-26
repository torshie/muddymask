#include <muddy/common/wrapper.hpp>
#include <muddy/common/mm/AlignedAllocator.hpp>

using namespace muddy;

void* AlignedAllocator::allocate(size_t size) {
	MUDDY_ASSERT(size % kPageSize == 0 && size > 0);
	void* pointer = wrapper::VirtualAlloc_(nullptr, size + alignment,
			MEM_RESERVE, PAGE_NOACCESS);
	uintptr_t base = reinterpret_cast<uintptr_t>(pointer);
	uintptr_t aligned = base / alignment * alignment + alignment;
	pointer = reinterpret_cast<void*>(aligned);
	wrapper::VirtualAlloc_(pointer, size, MEM_COMMIT, PAGE_READWRITE);
	return pointer;
}

void AlignedAllocator::release(void* p, size_t size) {
	MUDDY_ASSERT(size % kPageSize == 0 && size > 0);
	wrapper::VirtualFree_(p, size, MEM_DECOMMIT);
}
