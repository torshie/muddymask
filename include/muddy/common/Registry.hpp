#ifndef MUDDY_COMMON_REGISTRY_HPP_INCLUDED_
#define MUDDY_COMMON_REGISTRY_HPP_INCLUDED_

#if _WIN32

#include <list>
#include <string>
#include <windows.h>

namespace muddy {

class Registry {
public:
	static std::string getString(HKEY key, const char* path,
			const char* name);
	static std::list<std::string> listSubkey(HKEY key, const char* path);
};

} // namespace muddy

#endif // #if _WIN32

#endif // MUDDY_COMMON_REGISTRY_HPP_INCLUDED_
