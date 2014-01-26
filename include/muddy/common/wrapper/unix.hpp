#include <cerrno>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>

namespace muddy { namespace wrapper {

inline void* mmap_(void *addr, size_t length, int prot, int flags, int fd,
		off_t offset) {
	void* p = mmap(addr, length, prot, flags, fd, offset);
	if (MUDDY_UNLIKELY(p == MAP_FAILED)) {
		throw SystemError{errno};
	}
	return p;
}

inline void munmap_(void* p, size_t length) {
	INNER_ASSURE_ZERO(munmap(p, length));
}

template<typename... Args>
void ioctl_(int d, int req, Args... args) {
	INNER_ASSURE_ZERO(ioctl(d, req, args...));
}

inline int open_(const char *pathname, int flags) {
	MUDDY_ASSERT(!(flags & O_CREAT),
			"Missing the 3rd argument when creating file ", pathname);
	return INNER_ASSURE_NONNEGA(open(pathname, flags), "open(", pathname,
			")");
}

inline int open_(const char* pathname, int flags, mode_t mode) {
	return INNER_ASSURE_NONNEGA(open(pathname, flags, mode), "open(",
			pathname, ")");
}

inline void close_(int f) {
	INNER_ASSURE_ZERO(close(f));
}

inline ssize_t read_(int f, void* buf, size_t s) {
	return INNER_ASSURE_NONNEGA(read(f, buf, s));
}

inline ssize_t write_(int f, const void* buf, size_t s) {
	return INNER_ASSURE_NONNEGA(write(f, buf, s));
}

}} // namespace muddy::wrapper
