#include <muddy/common/wrapper.hpp>
#include <muddy/common/util/misc.hpp>

using namespace muddy;

void util::sleep(int millisecond) {
#if HAVE_SYSCALL_UNIX
	usleep(millisecond * 1000);
#elif HAVE_SYSCALL_WIN32
	Sleep(millisecond);
#else
#	error "Your platform isn't supported."
#endif
}
