#ifndef MUDDY_COMMON_MM_ARENA_ALLOCATOR_HPP_
#define MUDDY_COMMON_MM_ARENA_ALLOCATOR_HPP_

#include <cstddef>
#include <atomic>
#include <muddy/common/tpl/align.hpp>
#include <muddy/common/mm/AlignedAllocator.hpp>

namespace muddy {

template<int tObjectSize, int tAlignment>
class ArenaAllocator {
	static_assert(tObjectSize >= sizeof(void*),
			"Invalid template parameter tObjectSize");
public:
	static ArenaAllocator& single();

	void* allocate();
	void release(void* p);

private:
	enum {
		kArenaSize = 1024 * 1024,
	};

	struct StackNode {
		StackNode* next;

		static StackNode* push(StackNode* n, void* p) {
			static_cast<StackNode*>(p)->next = n;
			return static_cast<StackNode*>(p);
		}
	};

	AlignedAllocator source{kArenaSize};
	StackNode* local;
	std::atomic<StackNode*> global;

	ArenaAllocator() : local{nullptr}, global{nullptr} {}

	void append(void* arena);
};

template<int tObjectSize, int tAlignment>
void* ArenaAllocator<tObjectSize, tAlignment>::allocate() {
	if (MUDDY_UNLIKELY(local == nullptr)) {
		local = global.exchange(local);
	}
	if (MUDDY_UNLIKELY(local == nullptr)) {
		append(source.allocate(kArenaSize));
	}
	StackNode* p = local;
	local = local->next;
	return p;
};

template<int tObjectSize, int tAlignment>
void ArenaAllocator<tObjectSize, tAlignment>::release(void* p) {
	ArenaAllocator* allocator = *static_cast<ArenaAllocator**>(p);
	if (allocator == this) {
		local = StackNode::push(local, p);
		return;
	}

	StackNode* node = static_cast<StackNode*>(p);
	node->next = allocator->global;
	while (!allocator->global.compare_exchange_weak(node->next, node)) {
		// EMPTY
	}
}

template<int tObjectSize, int tAlignment>
void ArenaAllocator<tObjectSize, tAlignment>::append(void* arena) {
	*static_cast<ArenaAllocator**>(arena) = this;

	char* begin = tpl::align(static_cast<char*>(arena) + sizeof(void*),
			tAlignment);
	StackNode* l = local;
	while (begin < static_cast<char*>(arena) + kArenaSize - tObjectSize) {
		l = StackNode::push(l, begin);
		begin = tpl::align(begin + tObjectSize, tAlignment);
	}
	local = l;
}

} // namespace muddy

#endif // MUDDY_COMMON_MM_ARENA_ALLOCATOR_HPP_
