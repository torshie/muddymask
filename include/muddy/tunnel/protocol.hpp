#ifndef MUDDY_TUNNEL_PROTOCOL_HPP_INCLUDED_
#define MUDDY_TUNNEL_PROTOCOL_HPP_INCLUDED_

#include <muddy/common/compat/stdint.h>

namespace muddy {

enum {
	kGreeting = 1, kChallenge, kAnswer, kConfigure, kForwardData
};

template<int tType> struct Packet;

template<>
struct Packet<kGreeting> {
	uint64_t magic;
	char user[64];
};

template<>
struct Packet<kConfigure> {
	in_addr_t address;
	in_addr_t netmask;
	in_addr_t gateway;
	uint32_t _reserved;
	uint64_t cryptoKey;
};

template<>
struct Packet<kForwardData> {
	uint32_t checksum;
	char data[];
};

} // namespace muddy

#endif // MUDDY_TUNNEL_PROTOCOL_HPP_INCLUDED_
