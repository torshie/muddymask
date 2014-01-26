#ifndef MUDDY_COMMON_EXCEPT_BAD_ERRNO_HPP_
#define MUDDY_COMMON_EXCEPT_BAD_ERRNO_HPP_

#include <cstring>

namespace muddy {

class BadErrno : public IgnoredExcept {
public:
	template<typename... Info>
	explicit BadErrno(int e, Info... info)
			: IgnoredExcept{__func__, std::strerror(e), ", ", info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_BAD_ERRNO_HPP_
