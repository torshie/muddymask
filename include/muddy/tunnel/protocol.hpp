#ifndef MUDDY_TUNNEL_PROTOCOL_HPP_INCLUDED_
#define MUDDY_TUNNEL_PROTOCOL_HPP_INCLUDED_

#include <stdint.h>

namespace muddy {

enum PacketType {
	kStartSession = 1,
	kEndSession,
	kRelayData,
};

struct PacketHeader {
	int8_t type;
	int8_t _unused[3];
	int32_t length;
};

enum { kMaxIdle = 120 };

} // namespace muddy

#endif // MUDDY_TUNNEL_PROTOCOL_HPP_INCLUDED_
