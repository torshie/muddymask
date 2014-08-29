#include <cstring>
#include <muddy/common/wrapper.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/Registry.hpp>
#include <muddy/veth/Tuntap.hpp>

using namespace muddy;

#define TAP_WIN_CONTROL_CODE(request, method) \
	CTL_CODE(FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)
#define TAP_WIN_IOCTL_SET_MEDIA_STATUS \
	TAP_WIN_CONTROL_CODE (6, METHOD_BUFFERED)

Tuntap::Tuntap() {
	IP_ADAPTER_INFO info[32];
	DWORD size = sizeof(info);
	MUDDY_ASSERT(GetAdaptersInfo(info, &size) == 0);
	IP_ADAPTER_INFO* adapter = nullptr;
	if (size > 0) {
		IP_ADAPTER_INFO* p = info;
		while (p) {
			if (std::strcmp(p->Description, "TAP-Windows Adapter V9")
					== 0) {
				adapter = p;
				break;
			}
			p = p->Next;
		}
	}
	if (adapter == nullptr) {
		throw ConfigError{"Cannot find a TAP-Win32 adapter."};
	}

	std::memcpy(mac, adapter->Address, sizeof(mac));
	index = adapter->Index;
	static const char* const kDeviceSpace = "\\\\.\\Global\\";
	std::string path = std::string(kDeviceSpace) + adapter->AdapterName + ".tap";
	handle = wrapper::CreateFile_(path.c_str(),
			GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
			FILE_ATTRIBUTE_SYSTEM, 0);
}

void Tuntap::start(in_addr_t address, in_addr_t netmask) {
	DWORD len = 0;
	ULONG s = TRUE;
	wrapper::DeviceIoControl_(handle, TAP_WIN_IOCTL_SET_MEDIA_STATUS, &s,
			sizeof(s), &s, sizeof(s), &len, nullptr);

	ULONG context, instance;
	if (!AddIPAddress(address, netmask, index, &context, &instance)) {
		throw Unpossible{"AddIPAddress failed."};
	}
}

Tuntap::~Tuntap() {
	wrapper::CloseHandle_(handle);
}

int Tuntap::read(void* buffer, int size) {
	DWORD n;
	wrapper::ReadFile_(handle, buffer, size, &n, nullptr);
	return n;
}

int Tuntap::write(const void* buffer, int size) {
	DWORD n;
	wrapper::WriteFile_(handle, buffer, size, &n, nullptr);
	return n;
}
