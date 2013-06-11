#include <cstring>
#if _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <muddy/common/util/misc.hpp>
#include <muddy/common/wrapper/wrapper.hpp>
#include <muddy/common/Configuration.hpp>

namespace muddy { namespace util {

bool boolOptionSet(const Configuration& config, const char* name) {
	const char* value = config.getString(name);
	if (value == NULL)
		return false;
	return value[0] == '\0' || std::strcmp(value, "yes") == 0
			|| std::strcmp(value, "y") == 0
			|| std::strcmp(value, "true") == 0;
}

void pause() {
#if _WIN32
	Sleep(INFINITE);
#else
	::pause();
#endif
}

#if !_WIN32

void enableCoreDump() {
	struct rlimit r;
	wrapper::getrlimit_(RLIMIT_CORE, &r);
	r.rlim_cur = r.rlim_max;
	wrapper::setrlimit_(RLIMIT_CORE, &r);
}

#endif

}} // namespace muddy::util
