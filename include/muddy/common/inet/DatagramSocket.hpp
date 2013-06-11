#if _WIN32
#	include <muddy/common/inet/DatagramSocket_win32.hpp>
#else
#	include <muddy/common/inet/DatagramSocket_posix.hpp>
#endif
