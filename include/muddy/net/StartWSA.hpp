#ifndef MUDDY_NET_START_WSA_HPP_INCLUDED_
#define MUDDY_NET_START_WSA_HPP_INCLUDED_

#if HAVE_SOCK_WINSOCK2

#include <muddy/common/InitGlobal.hpp>

namespace muddy {

// This class does nothing but call WSAStartup() & WSACleanup()
class StartWSA {
	friend class InitGlobal<StartWSA>;

	static StartWSA& get();

	StartWSA() {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			throw Unpossible{"WSAStartup() failed"};
		}
	}

	~StartWSA() {
		WSACleanup();
	}

};

static InitGlobal<StartWSA> _initStartWSA;

} // namespace muddy

#endif // #if HAVE_SOCK_WIN32

#endif // MUDDY_NET_START_WSA_HPP_INCLUDED_
