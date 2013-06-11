#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <muddy/common/Configuration.hpp>
#include <muddy/common/InitManager.hpp>
#include <muddy/adapter/Tuntap.hpp>

using namespace muddy;

int main(int argc, char** argv) {
	InitManager manager;

	Configuration config("client");
	config.add("server", true).add("username", false, "guest")
			.parse(argc, argv);
	return 0;
}
