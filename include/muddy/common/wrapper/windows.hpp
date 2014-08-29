#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>

namespace muddy { namespace wrapper {

inline LPVOID VirtualAlloc_(LPVOID lpAddress, SIZE_T dwSize,
		DWORD flAllocationType, DWORD flProtect) {
	auto r = VirtualAlloc(lpAddress, dwSize, flAllocationType,
			flProtect);
	if (MUDDY_UNLIKELY(r == nullptr)) {
		throw SystemError(GetLastError());
	}
	return r;
}

inline void VirtualFree_(LPVOID lpAddress, SIZE_T dwSize,
		DWORD dwFreeType) {
	MUDDY_ASSURE_TRUE(VirtualFree(lpAddress, dwSize, dwFreeType));
}

inline HANDLE CreateFile_(LPCTSTR lpFileName, DWORD dwDesiredAccess,
		DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile) {
	auto r = CreateFile(lpFileName, dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition,
			dwFlagsAndAttributes, hTemplateFile);
	if (MUDDY_UNLIKELY(r == INVALID_HANDLE_VALUE)) {
		throw SystemError(GetLastError(), lpFileName);
	}
	return r;
}

inline void CloseHandle_(HANDLE h) {
	MUDDY_ASSURE_TRUE(CloseHandle(h));
}

inline void DeviceIoControl_(HANDLE hDevice, DWORD dwIoControlCode,
		LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer,
		DWORD nOutBufferSize, LPDWORD lpBytesReturned,
		LPOVERLAPPED lpOverlapped) {
	MUDDY_ASSURE_TRUE(DeviceIoControl(hDevice, dwIoControlCode,
			lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize,
			lpBytesReturned, lpOverlapped));
}

inline void ReadFile_(HANDLE hFile, LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped) {
	MUDDY_ASSURE_TRUE(ReadFile(hFile, lpBuffer, nNumberOfBytesToRead,
			lpNumberOfBytesRead, lpOverlapped));
}

inline void WriteFile_(HANDLE hFile, LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten,
		LPOVERLAPPED lpOverlapped) {
	MUDDY_ASSURE_TRUE(hFile, lpBuffer, nNumberOfBytesToWrite,
			lpNumberOfBytesWritten, lpOverlapped);
}

}} // namespace muddy::wrapper
