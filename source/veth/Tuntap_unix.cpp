#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <muddy/common/wrapper.hpp>
#include <muddy/veth/Tuntap.hpp>

using namespace muddy;

Tuntap::Tuntap(in_addr_t address, in_addr_t netmask) {
	(void)address;
	(void)netmask;

	struct ifreq ifr;
	handle = wrapper::open_("/dev/net/tun", O_RDWR);
	std::memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
	std::strcpy(ifr.ifr_name, "muddy0");
	wrapper::ioctl_(handle, TUNSETIFF, &ifr);
}

Tuntap::~Tuntap() {
	wrapper::close_(handle);
}

int Tuntap::read(void* buffer, int size) {
	return wrapper::read_(handle, buffer, size);
}

int Tuntap::write(const void* buffer, int size) {
	return wrapper::write_(handle, buffer, size);
}
