#include <cstdio>
#include <cstring>
#if _WIN32
#include <winsock2.h>
#endif
#include <muddy/common/compat/memory>
#include <muddy/common/wrapper/wrapper.hpp>
#include <muddy/common/util/inet.hpp>

namespace muddy { namespace util {

bool isInetAddress(const char* str) {
	unsigned int a, b, c, d, port;
	return std::sscanf(str, "%u.%u.%u.%u:%u", &a, &b, &c, &d, &port) == 5
			&& a <= 255 && b <= 255 && c <= 255 && d <= 255
			&& port <= 65535;
}

sockaddr_in getInetAddress(const char* string) {
	sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;

	shared_ptr<char> copy(strdup(string), &std::free);
	char* colon = std::strchr(copy.get(), ':');
	*colon = '\0';
	address.sin_addr.s_addr = wrapper::inet_addr_(copy.get());
	address.sin_port = htons(std::atoi(colon + 1));
	return address;
}

}} // namespace muddy::util
