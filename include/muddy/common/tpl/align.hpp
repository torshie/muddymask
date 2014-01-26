#ifndef MUDDY_COMMON_TPL_ALIGN_HPP_
#define MUDDY_COMMON_TPL_ALIGN_HPP_

#include <cstdint>

namespace muddy { namespace tpl {

template<typename T>
T* align(T* address, int alignment) {
	uintptr_t addr = reinterpret_cast<uintptr_t>(address);
	uintptr_t final = (addr + alignment - 1) / alignment * alignment;
	return reinterpret_cast<T*>(final);
}

}} // namespace muddy::tpl

#endif // MUDDY_COMMON_TPL_ALIGN_HPP_
