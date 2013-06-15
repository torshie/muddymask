#ifndef MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_
#define MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_

#ifndef MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <sys/types.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

namespace muddy { namespace wrapper {

inline void getrlimit_(int resource, struct rlimit* rlim) {
	CHECK_RETURN_ZERO(getrlimit(resource, rlim));
}

inline void setrlimit_(int resource, const struct rlimit* rlim) {
	CHECK_RETURN_ZERO(setrlimit(resource, rlim));
}

inline void setuid_(uid_t uid) {
	CHECK_RETURN_ZERO(setuid(uid));
}

inline void setgid_(gid_t gid) {
	CHECK_RETURN_ZERO(setgid(gid));
}

inline int open_(const char* path, int flags) {
	if (flags & O_CREAT) {
		throw InvalidArgument();
	}
	CHECK_NON_NEGATIVE(int, open(path, flags));
}

inline ssize_t read_(int fd, void* buf, size_t s) {
	CHECK_NON_NEGATIVE(ssize_t, read(fd, buf, s));
}

inline ssize_t write_(int fd, const void* buf, size_t s) {
	CHECK_NON_NEGATIVE(ssize_t, write(fd, buf, s));
}

inline void close_(int fd) {
	CHECK_RETURN_ZERO(close(fd));
}

}} // namespace muddy::wrapper

#endif // MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_
