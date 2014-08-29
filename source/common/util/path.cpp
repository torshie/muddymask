#include <muddy/config.h>
#include <cstring>
#include <muddy/common/util/define.hpp>
#include <muddy/common/util/path.hpp>

namespace {

const char kPathSeparator
#if HAVE_BACKWARD_PATH_SEPA
	{'\\'};
#elif HAVE_FORWARD_PATH_SEPA
	{'/'};
#else
#	error "Your platform isn't supported."
#endif

} // namespace

namespace muddy { namespace util {

const char* basename(const char* path) {
	const char* p = path + std::strlen(path) - 1;
	if (MUDDY_UNLIKELY(p < path)) {
		return path;
	}
	while (p > path && *p == kPathSeparator) {
		--p;
	}
	while (p > path && *p != kPathSeparator) {
		--p;
	}
	if (p == path) {
		return path;
	} else {
		return p + 1;
	}
}

}} // namespace muddy::util
