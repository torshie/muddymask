#ifndef MUDDY_VETH_TUNTAP_HPP_
#define MUDDY_VETH_TUNTAP_HPP_

#include <muddy/config.h>
#include <muddy/support/net.hpp>

namespace muddy {

class Tuntap {
public:
	Tuntap();
	~Tuntap();

	int read(void* buffer, int size);
	int write(const void* buffer, int size);
	void start(in_addr_t address, in_addr_t netmask);

	void getAddress(unsigned char* mac) const {
		std::memcpy(mac, this->mac, 6);
	}

private:
	unsigned char mac[6];
#if HAVE_TUNTAP_WIN32
	void* handle; // TODO Bad design to use type void* instead of HANDLE
	int index;
#elif HAVE_TUNTAP_LINUX
	int handle;
#else
#	error "Your platform isn't supported."
#endif
};

} // namespace muddy

#endif // MUDDY_VETH_TUNTAP_HPP_
