#ifndef MUDDY_COMMON_EXCEPT_SYSTEM_ERROR_UNIX_HPP_
#define MUDDY_COMMON_EXCEPT_SYSTEM_ERROR_UNIX_HPP_

#include <cstring>

namespace muddy {

class SystemError : public IgnoredExcept {
public:
	template<typename... Info>
	explicit SystemError(int e, Info... info)
			: IgnoredExcept{__func__, std::strerror(e), ", ", info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_SYSTEM_ERROR_UNIX_HPP_
