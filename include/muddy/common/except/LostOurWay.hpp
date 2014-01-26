#ifndef MUDDY_COMMON_EXCEPT_LOST_OUR_WAY_HPP_
#define MUDDY_COMMON_EXCEPT_LOST_OUR_WAY_HPP_

namespace muddy {

// This exception is usually thrown when the program reaches some code path
// it shouldn't.
class LostOurWay : public IgnoredExcept {
public:
	template<typename... Info>
	LostOurWay(Info... info) : IgnoredExcept{__func__, info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_LOST_OUR_WAY_HPP_
