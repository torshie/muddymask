#ifdef MUDDY_DECLARE_WINSOCK_INIT_HELPER_

#include <muddy/common/InitManager.hpp>

struct WinsockInitHelper {
	WinsockInitHelper() {
		::muddy::InitManager::setup(&initWinsock, &cleanupWinsock,
				::muddy::InitManager::kWinsock, false, "winsock2");
	}

	static void initWinsock() {
		WORD version = MAKEWORD(2, 2);
		WSADATA data;
		if (WSAStartup(version, &data) != 0) {
			throw ::muddy::Unpossible();
		}
	}

	static void cleanupWinsock() {
		WSACleanup();
	}
};
extern WinsockInitHelper winsockInitHelper;

#undef MUDDY_DECLARE_WINSOCK_INIT_HELPER_

#endif
