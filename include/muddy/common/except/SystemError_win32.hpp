#ifndef MUDDY_COMMON_EXCEPT_SYSTEM_ERROR_WIN32_HPP_
#define MUDDY_COMMON_EXCEPT_SYSTEM_ERROR_WIN32_HPP_

#include <winsock2.h>
#include <windows.h>

namespace muddy { namespace inner {

struct ErrorText {
	LPTSTR text = nullptr;

	explicit ErrorText(int code) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
						| FORMAT_MESSAGE_ALLOCATE_BUFFER,
				0, code, 0, reinterpret_cast<LPTSTR>(&text), 0,
				nullptr);
	}

	~ErrorText() {
		LocalFree(text);
	}
};

}} // namespace muddy::inner

namespace muddy {

class SystemError : private inner::ErrorText, public IgnoredExcept {
public:
	template<typename... Info>
	SystemError(int code, Info... info)
			: ErrorText{code},
			IgnoredExcept{__func__, text, ", ", info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_SYSTEM_ERROR_WIN32_HPP_
