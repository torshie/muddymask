#ifndef MUDDY_ADAPTER_TUNTAP_WIN32_HPP_INCLUDED_
#define MUDDY_ADAPTER_TUNTAP_WIN32_HPP_INCLUDED_

#include <muddy/common/util/define.hpp>
#include <muddy/common/wrapper/wrapper.hpp>

namespace muddy {

class Tuntap {
	DISABLE_COPY(Tuntap);
public:
	Tuntap(in_addr_t address, in_addr_t mask);
	~Tuntap();

	int read(void* buffer, int size);
	int write(const void* buffer, int size);

private:
	void* handle; // TODO Use void* instead of HANDLE is bad bad.
};

} // namespace muddy

#endif // MUDDY_ADAPTER_TUNTAP_WIN32_HPP_INCLUDED_
