#ifndef MUDDY_COMPAT_MISC_HPP_INCLUDED_
#define MUDDY_COMPAT_MISC_HPP_INCLUDED_

#include <config.h>
#include <stdint.h>

namespace muddy {

#ifndef HAVE_IN_ADDR_T
typedef uint32_t in_addr_t;
#endif

} // namespace muddy

#endif // MUDDY_COMPAT_MISC_HPP_INCLUDED_
