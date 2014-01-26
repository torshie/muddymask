#ifndef MUDDY_COMMON_WRAPPER_HPP_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>

namespace muddy { namespace inner { namespace wrapper {

template<typename Ret, typename... Args>
void assureTrue(Ret v, Args... args) {
	if (MUDDY_UNLIKELY(v == 0)) {
		throw WinApiError{args...};
	}
}

}}} // namespace muddy::inner::wrapper

#define INNER_ASSURE_TRUE(...) \
	::muddy::inner::wrapper::assureTrue(__VA_ARGS__)

#include <muddy/common/wrapper/win32.hpp>
#include <muddy/common/wrapper/imagehlp.hpp>

#undef INNER_ASSURE_TRUE
