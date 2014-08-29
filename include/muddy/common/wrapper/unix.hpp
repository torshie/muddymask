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
	MUDDY_ASSURE_ZERO(munmap(p, length));
}

template<typename... Args>
void ioctl_(int d, int req, Args... args) {
	MUDDY_ASSURE_ZERO(ioctl(d, req, args...));
}

inline int open_(const char *pathname, int flags) {
	MUDDY_ASSERT(!(flags & O_CREAT),
			"Missing the 3rd argument when creating file ", pathname);
	MUDDY_ASSURE_NATURAL(open(pathname, flags), "open(", pathname,
			")");
}

inline int open_(const char* pathname, int flags, mode_t mode) {
	MUDDY_ASSURE_NATURAL(open(pathname, flags, mode), "open(",
			pathname, ")");
}

inline void close_(int f) {
	MUDDY_ASSURE_ZERO(close(f));
}

inline ssize_t read_(int f, void* buf, size_t s) {
	MUDDY_ASSURE_NATURAL(read(f, buf, s));
}

inline ssize_t write_(int f, const void* buf, size_t s) {
	MUDDY_ASSURE_NATURAL(write(f, buf, s));
}

inline int dup_(int fd) {
	MUDDY_ASSURE_NATURAL(dup(fd));
}

inline void dup2_(int src, int tgt) {
	MUDDY_ASSURE_ZERO(dup2(src, tgt));
}

inline ssize_t writev_(int fd, const struct iovec *iov, int cnt) {
	MUDDY_ASSURE_NATURAL(writev(fd, iov, cnt));
}

}} // namespace muddy::wrapper
