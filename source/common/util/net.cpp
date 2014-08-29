#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <muddy/common/wrapper.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/except.hpp>
#include <muddy/common/util/net.hpp>

namespace muddy { namespace util {

sockaddr_in getInetAddress(const char* string) {
	const char* colon = std::strchr(string, ':');
	if (colon == nullptr || colon == string) {
		throw ConfigError{"invalid address string: ", string};
	}
	int port;
	if (std::sscanf(colon + 1, "%d", &port) != 1
			|| port <= 0 || port >= 0x10000 - 1) {
		throw ConfigError{"invalid port number: ", colon + 1};
	}

	char copy[colon - string + 1];
	std::memcpy(copy, string, colon - string);
	copy[sizeof(copy) - 1] = '\0';
	auto host = gethostbyname(copy);
	if (host == nullptr || host->h_addr_list[0] == nullptr) {
		throw ConfigError{"gethostbyname() failed, ", copy};
	}

	sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr =
			*reinterpret_cast<in_addr_t*>(host->h_addr_list[0]);
	address.sin_port = htons(port);
	return address;
}

}} // namespace muddy::util
