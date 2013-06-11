#ifndef MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_
#define MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_

#ifndef MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

namespace muddy { namespace wrapper {

inline void getrlimit_(int resource, struct rlimit* rlim) {
	CHECK_RETURN_ZERO(getrlimit(resource, rlim));
}

inline void setrlimit_(int resource, const struct rlimit* rlim) {
	CHECK_RETURN_ZERO(setrlimit(resource, rlim));
}

}} // namespace muddy::wrapper

#endif // MUDDY_COMMON_WRAPPER_UNIX_HPP_INCLUDED_
