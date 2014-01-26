#ifndef MUDDY_VETH_TUNTAP_HPP_
#define MUDDY_VETH_TUNTAP_HPP_

#if HAVE_MUDDY_CONFIG_H
#	include <muddy/config.h>
#endif

#include <muddy/support/net.hpp>

namespace muddy {

class Tuntap {
public:
	Tuntap(in_addr_t address, in_addr_t netmask);
	~Tuntap();

	int read(void* buffer, int size);
	int write(const void* buffer, int size);

private:
#if HAVE_TUNTAP_WIN32
	void* handle; // TODO Bad design to use type void* instead of HANDLE
#elif HAVE_TUNTAP_LINUX
	int handle;
#else
#	error "Your platform isn't supported."
#endif
};

} // namespace muddy

#endif // MUDDY_VETH_TUNTAP_HPP_
