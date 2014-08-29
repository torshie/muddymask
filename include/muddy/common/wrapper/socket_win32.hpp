namespace muddy { namespace wrapper {

inline void bind_(SocketHandle s, const void* addr, socklen_t len) {
	if (bind(s, static_cast<const sockaddr*>(addr), len) != 0) {
		throw SystemError{WSAGetLastError()};
	}
}

inline ssize_t recvfrom_(SocketHandle sock, void* buf, size_t len,
		int flags, sockaddr_in* addr, socklen_t* addrlen) {
	int r = recvfrom(sock, static_cast<char*>(buf), len, flags,
			reinterpret_cast<sockaddr*>(addr), addrlen);
	if (r == SOCKET_ERROR) {
		throw SystemError{WSAGetLastError()};
	}
	return r;
}

inline ssize_t sendto_(SocketHandle sock, const void* buf, size_t len,
		int flags, const sockaddr_in* addr, socklen_t addrlen) {
	int r = sendto(sock, static_cast<const char*>(buf), len, flags,
			reinterpret_cast<const sockaddr*>(addr), addrlen);
	if (r == SOCKET_ERROR) {
		throw SystemError{WSAGetLastError()};
	}
	return r;
}

}} // namespce muddy::wrapper
