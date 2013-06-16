#include <cstring>
#include <utility>
#if _WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <muddy/common/logging/log.hpp>
#include <muddy/common/util/inet.hpp>
#include <muddy/common/util/misc.hpp>
#include <muddy/common/wrapper/wrapper.hpp>
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

#if !_WIN32
void dropRoot() {
	passwd* pw = getpwnam("nobody");
	if (pw == NULL) {
		WARNING << "Cannot find system user \"nobody\"";
		return;
	}
	wrapper::setgid_(pw->pw_gid);
	wrapper::setuid_(pw->pw_uid);
}
#endif

class MuddyServer {
public:
	MuddyServer(Tuntap* t, DatagramSocket* s)
			: tuntap(t), socket(s), inbound(NULL), outbound(NULL),
			keepGoing(true) {}

	void startup() {
		inbound = new Thread(&handleIncomingData, this);
		outbound = new Thread(&handleOutgoingData, this);
	}

	void shutdown() {
		keepGoing = false;
		inbound->wait();
		outbound->wait();
		delete this; // TODO "delete this" is OK, but ?
	}

private:
	Tuntap* tuntap;
	DatagramSocket* socket;
	Thread* inbound;
	Thread* outbound;
	volatile bool keepGoing;

	~MuddyServer() {
		delete inbound;
		delete outbound;
	}

	static void handleIncomingData(void* arg);
	static void handleOutgoingData(void* arg);
};

} // namespace

int main(int argc, char** argv) {
	InitManager manager;

	Configuration config("server");
	config.add("listen", false, "0.0.0.0:5303").add("network", true)
			.add("secret", false, "");
#if !_WIN32
	config.add("daemon", false).add("core", false);
#endif
	config.parse(argc, argv);

#if !_WIN32
	if (util::boolOptionSet(config, "core")) {
		util::enableCoreDump();
	}
	if (util::boolOptionSet(config, "daemon")) {
		wrapper::daemon_(1, 1);
	}
#endif

	char copy[64] = { 0 };
	std::strncpy(copy, config.getString("network"), sizeof(copy) - 1);
	char* prefix = splitNetworkString(copy);
	in_addr_t netmask = getNetMask(prefix);
	in_addr_t local = wrapper::inet_addr_(copy);

	// This following two objects would be accessed by multiple threads,
	// so it cannot be created on the stack of the main thread.
	std::auto_ptr<Tuntap> tuntap(new Tuntap(local, netmask));
	std::auto_ptr<DatagramSocket> socket(new DatagramSocket());
	socket->bind(util::getInetAddress(config.getString("listen")));

#if !_WIN32
	dropRoot();
#endif
	MuddyServer* server = new MuddyServer(tuntap.get(), socket.get());
	server->startup();

	util::pause();

	server->shutdown();
}
