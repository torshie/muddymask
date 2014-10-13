#include <iomanip>
#include <muddy/common/logging/log.hpp>
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

void util::hexdump(const void* data, int size) {
	auto p = static_cast<const uint8_t*>(data);
	MUDDY_DEBUG << "\n" << logging::continuous;
	for (int i = 0; i < size; ++i) {
		if (i % 16 == 0) {
			MUDDY_DEBUG << std::setfill('0') << std::setw(8) << std::hex
					<< i << std::setw(2) << ": " << logging::continuous;
		}
		MUDDY_DEBUG << std::setfill('0') << std::setw(2) << (int)p[i]
				<< std::setw(1) << " " << logging::continuous;
		if (i % 16 == 15) {
			MUDDY_DEBUG << "\n" << logging::continuous;
		} else if (i % 16 == 7) {
			MUDDY_DEBUG << "| " << logging::continuous;
		}
	}
	MUDDY_DEBUG << "" << std::resetiosflags(std::ios_base::basefield);
}
