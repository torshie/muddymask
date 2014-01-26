#ifndef MUDDY_VETH_TUNTAP_HPP_
#define MUDDY_VETH_TUNTAP_HPP_

#include <muddy/support/net.hpp>

namespace muddy {

class Tuntap {
public:
	Tuntap(in_addr_t address, in_addr_t netmask);
	~Tuntap();

	int read(void* buffer, int size);
	int write(const void* buffer, int size);

private:
#if _WIN32
	void* handle; // TODO Bad design to use type void* instead of HANDLE
#else
	int handle;
#endif
};

} // namespace muddy

#endif // MUDDY_VETH_TUNTAP_HPP_
