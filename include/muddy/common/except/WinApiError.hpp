#ifndef MUDDY_COMMON_EXCEPT_WIN_API_ERROR_HPP_
#define MUDDY_COMMON_EXCEPT_WIN_API_ERROR_HPP_

#include <windows.h>

namespace muddy { namespace inner {

struct LastErrorText {
	LPTSTR text = nullptr;

	LastErrorText() {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
						| FORMAT_MESSAGE_ALLOCATE_BUFFER,
				0, GetLastError(), 0, reinterpret_cast<LPTSTR>(&text), 0,
				nullptr);
	}

	~LastErrorText() {
		LocalFree(text);
	}
};

}} // namespace muddy::inner

namespace muddy {

class WinApiError : private inner::LastErrorText, public IgnoredExcept {
public:
	template<typename... Info>
	WinApiError(Info... info)
			: IgnoredExcept{__func__, text, ", ", info...} {}
};

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_WIN_API_ERROR_HPP_
