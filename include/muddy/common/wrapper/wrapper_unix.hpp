#ifndef MUDDY_COMMON_WRAPPER_HPP_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <cerrno>
#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>

namespace muddy { namespace inner { namespace wrapper {

template<typename Ret, typename... Args>
void assureZero(Ret v, Args... args) {
	if (MUDDY_UNLIKELY(v != 0)) {
		throw BadErrno(errno, args...);
	}
}

template<typename Ret, typename... Args>
Ret assureNonnega(Ret v, Args... args) {
	if (MUDDY_UNLIKELY(v < 0)) {
		throw BadErrno(errno, args...);
	}
	return v;
}

}}} // namespace muddy::inner::wrapper

#define INNER_ASSURE_ZERO(...) \
	::muddy::inner::wrapper::assureZero(__VA_ARGS__)
#define INNER_ASSURE_NONNEGA(...) \
	::muddy::inner::wrapper::assureNonnega(__VA_ARGS__)

#include <muddy/common/wrapper/unix.hpp>

#undef INNER_ASSURE_ZERO
#undef INNER_ASSURE_NONNEGA
