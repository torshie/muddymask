#include <muddy/common/wrapper.hpp>
#include <muddy/common/Registry.hpp>
#include <muddy/veth/Tuntap.hpp>

using namespace muddy;

#define TAP_WIN_CONTROL_CODE(request, method) \
	CTL_CODE(FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)
#define TAP_WIN_IOCTL_SET_MEDIA_STATUS \
	TAP_WIN_CONTROL_CODE (6, METHOD_BUFFERED)
#define TAP_WIN_IOCTL_CONFIG_TUN \
	TAP_WIN_CONTROL_CODE(10, METHOD_BUFFERED)

namespace {

std::string getDeviceId() {
	static const char* const kAdapterKey = "SYSTEM\\CurrentControlSet"
			"\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}";
	std::list<std::string> k = Registry::getSubkey(HKEY_LOCAL_MACHINE,
			kAdapterKey);
	std::string base = std::string(kAdapterKey) + "\\";
	for (const auto& i : k) {
		std::string path = base + i;
		std::string id = Registry::getString(HKEY_LOCAL_MACHINE,
				path.c_str(), "ComponentId");
		if (id == "tap0901") {
			return Registry::getString(HKEY_LOCAL_MACHINE, path.c_str(),
					"NetCfgInstanceId");
		}
	}
	throw LostOurWay{};
}

} // namespace

Tuntap::Tuntap(in_addr_t address, in_addr_t netmask) {
	static const char* const kDeviceSpace = "\\\\.\\Global\\";
	std::string path = std::string(kDeviceSpace) + getDeviceId() + ".tap";
	handle = wrapper::CreateFile_(path.c_str(),
			GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_SYSTEM, 0);

	in_addr_t netcfg[3] = {address, address & netmask, netmask};
	netcfg[1] = netcfg[0] & netcfg[2];
	DWORD len = 0;
	wrapper::DeviceIoControl_(handle, TAP_WIN_IOCTL_CONFIG_TUN, netcfg,
			sizeof(netcfg), netcfg, sizeof(netcfg), &len, NULL);

	ULONG s = TRUE;
	wrapper::DeviceIoControl_(handle, TAP_WIN_IOCTL_SET_MEDIA_STATUS, &s,
			sizeof(s), &s, sizeof(s), &len, NULL);
}

Tuntap::~Tuntap() {
	wrapper::CloseHandle_(handle);
}

int Tuntap::read(void* buffer, int size) {
	DWORD n;
	wrapper::ReadFile_(handle, buffer, size, &n, NULL);
	return n;
}

int Tuntap::write(const void* buffer, int size) {
	DWORD n;
	wrapper::WriteFile_(handle, buffer, size, &n, NULL);
	return n;
}
