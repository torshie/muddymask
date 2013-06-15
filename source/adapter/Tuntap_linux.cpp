#include <sys/ioctl.h>
#include <muddy/common/wrapper/wrapper.hpp>
#include <muddy/adapter/Tuntap_linux.hpp>

using namespace muddy;

Tuntap::Tuntap(in_addr_t address, in_addr_t netmask) {
	(void)address;
	(void)netmask;

	struct ifreq ifr;
	tuntap = wrapper::open_("/dev/net/tun", O_RDWR);
	std::memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
	std::strcpy(ifr.ifr_name, "muddy0");

	if (ioctl(tuntap, TUNSETIFF, &ifr) != 0) {
		throw SystemError(errno);
	}
}

Tuntap::~Tuntap() {
	wrapper::close_(tuntap);
}

int Tuntap::read(void* buffer, int size) {
	return wrapper::read_(tuntap, buffer, size);
}

int Tuntap::write(const void* buffer, int size) {
	return wrapper::write_(tuntap, buffer, size);
}
