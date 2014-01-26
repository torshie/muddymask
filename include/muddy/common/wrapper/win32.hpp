#ifndef MUDDY_COMMON_WRAPPER_HPP_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <windows.h>
#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>

namespace muddy { namespace wrapper {

inline LPVOID VirtualAlloc_(LPVOID lpAddress, SIZE_T dwSize,
		DWORD flAllocationType, DWORD flProtect) {
	auto r = VirtualAlloc(lpAddress, dwSize, flAllocationType,
			flProtect);
	if (MUDDY_UNLIKELY(r == nullptr)) {
		throw WinApiError{};
	}
	return r;
}

inline void VirtualFree_(LPVOID lpAddress, SIZE_T dwSize,
		DWORD dwFreeType) {
	INNER_ASSURE_TRUE(VirtualFree(lpAddress, dwSize, dwFreeType));
}

inline HANDLE CreateFile_(LPCTSTR lpFileName, DWORD dwDesiredAccess,
		DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile) {
	auto r = CreateFile(lpFileName, dwDesiredAccess, dwShareMode,
			lpSecurityAttributes, dwCreationDisposition,
			dwFlagsAndAttributes, hTemplateFile);
	if (MUDDY_UNLIKELY(r == INVALID_HANDLE_VALUE)) {
		throw WinApiError{lpFileName};
	}
	return r;
}

inline void CloseHandle_(HANDLE h) {
	INNER_ASSURE_TRUE(CloseHandle(h));
}

inline void DeviceIoControl_(HANDLE hDevice, DWORD dwIoControlCode,
		LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer,
		DWORD nOutBufferSize, LPDWORD lpBytesReturned,
		LPOVERLAPPED lpOverlapped) {
	INNER_ASSURE_TRUE(DeviceIoControl(hDevice, dwIoControlCode,
			lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize,
			lpBytesReturned, lpOverlapped));
}

inline void ReadFile_(HANDLE hFile, LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped) {
	INNER_ASSURE_TRUE(ReadFile(hFile, lpBuffer, nNumberOfBytesToRead,
			lpNumberOfBytesRead, lpOverlapped));
}

inline void WriteFile_(HANDLE hFile, LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten,
		LPOVERLAPPED lpOverlapped) {
	INNER_ASSURE_TRUE(hFile, lpBuffer, nNumberOfBytesToWrite,
			lpNumberOfBytesWritten, lpOverlapped);
}

}} // namespace muddy::wrapper
