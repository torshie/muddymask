#ifndef MUDDY_COMMON_TPL_CONCAT_HPP_
#define MUDDY_COMMON_TPL_CONCAT_HPP_

#include <sstream>
#include <string>
#include <utility>

namespace muddy { namespace inner { namespace tpl {

template<typename T, typename... Types>
struct Concat {
	using Stream = std::ostringstream;

	static Stream& concat(Stream& stream, const T& v,
			const Types&... values) {
		stream << v;
		return Concat<Types...>::concat(stream, values...);
	}
};

template<typename T>
struct Concat<T> {
	using Stream = std::ostringstream;

	static Stream& concat(Stream& stream, const T& v) {
		stream << v;
		return stream;
	}
};

}}} // namespace muddy::inner::tpl

namespace muddy { namespace tpl {

template<typename... Types>
std::string concat(Types... values) {
	std::ostringstream stream;
	inner::tpl::Concat<Types...>::concat(stream, values...);
	return std::move(stream.str());
}

}} // namespace muddy::tpl

#endif // MUDDY_COMMON_TPL_CONCAT_HPP_
