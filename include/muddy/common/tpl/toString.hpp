#ifndef MUDDY_COMMON_TPL_TO_STRING_HPP_
#define MUDDY_COMMON_TPL_TO_STRING_HPP_

#include <string>

namespace muddy { namespace tpl {

template<typename T>
std::string toString(T v) {
	return std::to_string(v);
}

inline std::string toString(const std::string& s) {
	return s;
}

inline std::string toString(const char* s) {
	return std::string{s};
}

inline std::string toString(bool b) {
	return std::string{b ? "true" : "false"};
}

inline std::string toString(char c) {
	return std::string{1, c};
}

}} // namespace muddy::tpl

#endif // MUDDY_COMMON_TPL_TO_STRING_HPP_
