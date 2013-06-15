#ifndef MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_
#define MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_

#ifndef MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <sys/types.h>
#include <sys/resource.h>
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

}} // namespace muddy::wrapper

#endif // MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_
