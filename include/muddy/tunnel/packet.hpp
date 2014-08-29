#ifndef MUDDY_TUNNEL_PACKET_HEADER_HPP_INCLUDED_
#define MUDDY_TUNNEL_PACKET_HEADER_HPP_INCLUDED_

#include <cstdint>
#include <initializer_list>

namespace muddy {

struct [[gnu::packed]] PacketHeader {
	enum Type : uint8_t {
		kHeartbeat = 1, kStartSession, kSessionInfo, kCloseSession,
		kRelayData, kTypeCount
	};

	Type type;
	uint8_t _zero = 0;
	uint16_t length;
};

struct [[gnu::packed]] InetAddress {
	in_addr_t addr;
	uint16_t port;
	uint16_t _padding = 0;

	InetAddress() : addr(0), port(0) {}

	InetAddress(const sockaddr_in& that)
			: addr(that.sin_addr.s_addr), port(that.sin_port) {}

	const InetAddress& operator = (const sockaddr_in& that) {
		addr = that.sin_addr.s_addr;
		port = that.sin_port;
		_padding= 0;
		return *this;
	}

	bool operator == (const InetAddress& that) const {
		return addr == that.addr && port == that.port;
	}

	bool operator != (const InetAddress& that) const {
		return !(*this == that);
	}

	operator sockaddr_in () const {
		struct sockaddr_in in;
		std::memset(&in, sizeof(in), 0);
		in.sin_family = AF_INET;
		in.sin_port = port;
		in.sin_addr.s_addr = addr;
		return in;
	}
};
static_assert(sizeof(InetAddress) == 8, "Wrong InetAddress size");

struct [[gnu::packed]] MacAddress {
	uint8_t addr[6];
	uint16_t _padding;

	bool operator == (const MacAddress& that) const {
		return std::memcmp(addr, that.addr, 6) == 0;
	}

	bool operator != (const MacAddress& that) const {
		return !(*this == that);
	}
};

struct [[gnu::packed]] StartSession {
	MacAddress mac;
};

struct [[gnu::packed]] SessionInfo {
	in_addr_t address;
	in_addr_t gateway;
	in_addr_t netmask;
};

struct [[gnu::packed]] Heartbeat {
	uint32_t i;
};

struct [[gnu::packed]] EthernetHeader {
	MacAddress dst;
	MacAddress src;
	union {
		uint16_t type;
		uint16_t length;
	};
};

} // namespace muddy

#endif // MUDDY_TUNNEL_PACKET_HEADER_HPP_INCLUDED_
