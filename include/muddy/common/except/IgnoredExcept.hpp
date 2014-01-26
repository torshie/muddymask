#ifndef MUDDY_COMMON_EXCEPT_IGNORED_EXCEPT_HPP_
#define MUDDY_COMMON_EXCEPT_IGNORED_EXCEPT_HPP_

#include <muddy/common/tpl/concat.hpp>
#include <muddy/common/StackTrace.hpp>

namespace muddy {

class IgnoredExcept : public std::exception {
public:
	virtual const char* what() const noexcept {
		return message.c_str();
	}

protected:
	std::string message;

	template<typename T, typename... Info>
	IgnoredExcept(T t, Info... info);

private:
	static std::string getStackTrace(const StackTrace& trace);
};

template<typename T, typename... Info>
IgnoredExcept::IgnoredExcept(T t, Info... info)
		: message{tpl::concat("Exception: ", t, ", ", info..., '\n',
				"Backtrace:\n")} {
	StackTrace trace{3};
	message.append(getStackTrace(trace));
}

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_IGNORED_EXCEPT_HPP_
