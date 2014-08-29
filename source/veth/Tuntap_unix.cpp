#include <cstring>
#include <memory>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <muddy/common/wrapper.hpp>
#include <muddy/veth/Tuntap.hpp>

using namespace muddy;

Tuntap::Tuntap() {
	struct ifreq ifr;
	handle = wrapper::open_("/dev/net/tun", O_RDWR);
	std::memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	std::strcpy(ifr.ifr_name, "muddy0");
	wrapper::ioctl_(handle, TUNSETIFF, &ifr);
	std::memset(mac, 0, sizeof(mac));

	std::unique_ptr<FILE, void (*)(std::FILE *)> f(
			wrapper::fopen_("/sys/class/net/muddy0/address", "r"),
			wrapper::fclose_);
	if (f == nullptr) {
		return;
	}
	fscanf(f.get(), "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
			mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
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

void Tuntap::start(in_addr_t address, in_addr_t netmask) {
	struct ifreq ifr;
	sockaddr_in addr;
	std::memset(&ifr, 0, sizeof(ifr));
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = address;
	std::strcpy(ifr.ifr_name, "muddy0");
	std::memcpy(&(ifr.ifr_addr), &addr, sizeof(addr));

	int s = wrapper::socket_(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	wrapper::ioctl_(s, SIOCSIFADDR, &ifr);
	addr.sin_addr.s_addr = netmask;
	std::memcpy(&(ifr.ifr_addr), &addr, sizeof(addr));
	wrapper::ioctl_(s, SIOCSIFNETMASK, &ifr);
	wrapper::ioctl_(s, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
	wrapper::ioctl_(s, SIOCSIFFLAGS, &ifr);
	ifr.ifr_mtu = 1200;
	wrapper::ioctl_(s, SIOCSIFMTU, &ifr);
	wrapper::close_(s);
}
