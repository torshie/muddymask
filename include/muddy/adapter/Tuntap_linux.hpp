#ifndef MUDDY_ADAPTER_TUNTAP_LINUX_HPP_INCLUDED_
#define MUDDY_ADAPTER_TUNTAP_LINUX_HPP_INCLUDED_

#include <muddy/common/wrapper/wrapper.hpp>
#include <muddy/common/util/define.hpp>

namespace muddy {

class Tuntap {
	DISABLE_COPY(Tuntap);
public:
	Tuntap(in_addr_t address, in_addr_t netmask);
	~Tuntap();

	int read(void* buffer, int size);
	int write(const void* buffer, int size);

private:
	int tuntap;
};

} // namespace muddy

#endif // MUDDY_ADAPTER_TUNTAP_LINUX_HPP_INCLUDED_
