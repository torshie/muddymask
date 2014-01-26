#ifndef MUDDY_SUPPORT_NET_HPP_
#define MUDDY_SUPPORT_NET_HPP_

#ifdef HAVE_MUDDY_CONFIG_H
#	include <muddy/config.h>
#endif // #if HAVE_MUDDY_CONFIG_H

#include <cstdint>

#if HAVE_SOCK_UNIX
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>
#elif HAVE_SOCK_WINSOCK2
#include <ws2tcpip.h>
using in_addr_t = uint32_t;
using in_port_t = uint16_t;
#else
#	error "Your platform isn't supported."
#endif // #if HAVE_SOCK_UNIX

namespace muddy {

using SocketHandle = decltype(socket(0, 0, 0));

} // namespace muddy

#endif // MUDDY_SUPPORT_NET_HPP_
