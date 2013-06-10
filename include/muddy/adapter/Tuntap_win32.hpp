#ifndef MUDDY_ADAPTER_TUNTAP_WIN32_HPP_INCLUDED_
#define MUDDY_ADAPTER_TUNTAP_WIN32_HPP_INCLUDED_

#include <cstddef>
#include <windows.h>

namespace muddy {

class Tuntap {
public:
	Tuntap();
	~Tuntap();

	int read(void* buffer, int size);
	int write(const void* buffer, int size);

private:
	HANDLE handle;
};

} // namespace muddy

#endif // MUDDY_ADAPTER_TUNTAP_WIN32_HPP_INCLUDED_
