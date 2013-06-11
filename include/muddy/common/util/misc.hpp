#ifndef MUDDY_COMMON_UTIL_MISC_HPP_INCLUDED_
#define MUDDY_COMMON_UTIL_MISC_HPP_INCLUDED_

namespace muddy { class Configuration; }

namespace muddy { namespace util {

bool boolOptionSet(const Configuration& config, const char* name);
void pause();

#if !_WIN32
void enableCoreDump();
#endif

}} // namespace muddy::util

#endif // MUDDY_COMMON_UTIL_MISC_HPP_INCLUDED_
