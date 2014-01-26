#ifndef MUDDY_COMMON_EXCEPT_INVALID_ARGUMENT_HPP_
#define MUDDY_COMMON_EXCEPT_INVALID_ARGUMENT_HPP_

namespace muddy {

class InvalidArgument : public IgnoredExcept {
public:
	template<typename... Info>
	InvalidArgument(Info... info) : IgnoredExcept{__func__, info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_INVALID_ARGUMENT_HPP_
