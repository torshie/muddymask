#ifndef MUDDY_COMMON_EXCEPT_UNPOSSIBLE_HPP_
#define MUDDY_COMMON_EXCEPT_UNPOSSIBLE_HPP_

namespace muddy {

class Unpossible : public IgnoredExcept {
public:
	template<typename... Info>
	Unpossible(Info... info) : IgnoredExcept{__func__, info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_UNPOSSIBLE_HPP_
