#ifndef MUDDY_COMMON_EXCEPT_WRONG_ASSERTION_HPP_
#define MUDDY_COMMON_EXCEPT_WRONG_ASSERTION_HPP_

namespace muddy {

class WrongAssertion : public IgnoredExcept {
public:
	template<typename... Info>
	WrongAssertion(Info... info) : IgnoredExcept{__func__, info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_WRONG_ASSERTION_HPP_
