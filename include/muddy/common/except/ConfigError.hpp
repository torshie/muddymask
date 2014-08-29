#ifndef MUDDY_COMMON_EXCEPT_CONFIG_ERROR_HPP_INCLUDED_
#define MUDDY_COMMON_EXCEPT_CONFIG_ERROR_HPP_INCLUDED_

namespace muddy {

class ConfigError : public IgnoredExcept {
public:
	template<typename... Info>
	ConfigError(Info... info) : IgnoredExcept(__func__, info...) {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_CONFIG_ERROR_HPP_INCLUDED_
