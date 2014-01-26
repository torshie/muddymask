#ifndef MUDDY_SUPPORT_NET_HPP_
#define MUDDY_SUPPORT_NET_HPP_

#include <cstdint>

#if MUDDY_HAS_IN_ADDR_T
#include <netinet/in.h>
#else
using in_addr_t = uint32_t;
#endif

#endif // MUDDY_SUPPORT_NET_HPP_
