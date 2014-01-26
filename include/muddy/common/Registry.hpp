#ifndef MUDDY_COMMON_REGISTRY_HPP_
#define MUDDY_COMMON_REGISTRY_HPP_

#include <list>
#include <string>
#include <winsock2.h>
#include <windows.h>

namespace muddy {

class Registry {
public:
	static std::string getString(HKEY key, const char* path,
			const char* name);
	static std::list<std::string> getSubkey(HKEY key, const char* path);
};

} // namespace muddy

#endif // MUDDY_COMMON_REGISTRY_HPP_
