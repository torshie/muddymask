#include <windows.h>
#include <muddy/common/except.hpp>
#include <muddy/common/Registry.hpp>

using namespace muddy;

// TODO Do not throw instances of Unpssobile, throw WinApiError instead,
// or add relevant functions into namespace muddy::wrapper.

std::string Registry::getString(HKEY key, const char* path,
		const char* name) {
	char buffer[1024];
	DWORD size = sizeof(buffer);
	DWORD type;
	LONG r = RegGetValue(key, path, name, RRF_RT_REG_SZ, &type, buffer,
			&size);
	if (r != 0) {
		return std::string{};
	} else {
		return std::string{buffer};
	}
}

std::list<std::string> Registry::getSubkey(HKEY key, const char* path) {
	std::list<std::string> result;
	char buffer[256];
	DWORD size = sizeof(buffer);

	HKEY subkey;
	if (RegOpenKeyEx(key, path, 0, KEY_READ, &subkey) != 0) {
		throw Unpossible{"RegOpenKeyEx()", path};
	}

	DWORD index = 0;
	LONG r = RegEnumKeyEx(subkey, index++, buffer, &size,
			NULL, NULL, NULL, NULL);
	while (r == 0) {
		result.push_back(std::string(buffer));
		size = sizeof(buffer);
		r = RegEnumKeyEx(subkey, index++, buffer, &size, NULL, NULL, NULL,
				NULL);
	}
	if (r != ERROR_NO_MORE_ITEMS) {
		throw Unpossible{"RegEnumKeyEx()"};
	}
	return result;
}
