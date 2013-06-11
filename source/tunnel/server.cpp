#include <cstring>
#include <utility>
#include <winsock2.h>
#include <windows.h>
#include <muddy/common/util/inet.hpp>
#include <muddy/common/util/misc.hpp>
#include <muddy/common/Configuration.hpp>
#include <muddy/common/InitManager.hpp>
#include <muddy/common/inet/DatagramSocket.hpp>
#include <muddy/common/thread/Thread.hpp>
#include <muddy/adapter/Tuntap.hpp>

using namespace muddy;

namespace {

char* splitNetworkString(char* network) {
	char* slash = std::strchr(network, '/');
	if (slash == NULL) {
		throw ConfigError("invalid network %s", network);
	}
	*slash = '\0';
	++slash;
	if (*slash == '\0') {
		throw ConfigError("network id missing %s", network);
	}
	return slash;
}

in_addr_t getNetMask(const char* prefix) {
	int size;
	if (std::sscanf(prefix, " %d ", &size) != 1
			|| size < 1 || size > 31) {
		throw ConfigError("invalid network prefix \"%s\"", prefix);
	}
	return (~(in_addr_t)0) << (32 - size);
}

bool keepGoing = true;

void handleInboundData(void* arg) {
}

void handleOutboundData(void* arg) {
}

} // namespace

int main(int argc, char** argv) {
	InitManager manager;

	Configuration config("server");
	config.add("listen", false, "0.0.0.0:5303").add("network", true)
			.add("secret", true);
#if !_WIN32
	config.add("daemon", false).add("core", false);
#endif
	config.parse(argc, argv);

#if !_WIN32
	if (boolOptionSet(config, "core")) {
		enableCoreDump();
	}
	if (boolOptionSet(config, "daemon")) {
		wrapper::daemon_(1, 1);
	}
#endif

	char copy[64] = { 0 };
	std::strncpy(copy, config.getString("network"), sizeof(copy) - 1);
	char* prefix = splitNetworkString(copy);
	in_addr_t netmask = getNetMask(prefix);
	in_addr_t local = wrapper::inet_addr_(copy);

	// This tuntap instance would in accessed by multiple threads, so it
	// cannot be created on the stack of the main thread.
	std::auto_ptr<Tuntap> tuntap(new Tuntap(local, netmask));
	std::auto_ptr<DatagramSocket> socket(new DatagramSocket());
	socket->bind(util::getInetAddress(config.getString("listen")));

	typedef std::pair<void*, void*> Pair;
	std::auto_ptr<Pair> a(new Pair(tuntap.get(), socket.get()));
	std::auto_ptr<Thread> inboundThread(
			new Thread(&handleInboundData, a.get()));
	std::auto_ptr<Thread> outboundThread(
			new Thread(&handleOutboundData, a.get()));

	util::pause();

	keepGoing = false;
	inboundThread->wait();
	outboundThread->wait();
}
