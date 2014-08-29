namespace muddy { namespace wrapper {

inline void bind_(SocketHandle s, const void* addr, socklen_t len) {
	MUDDY_ASSURE_ZERO(bind(s, static_cast<const sockaddr*>(addr), len));
}

inline ssize_t recvfrom_(SocketHandle sock, void* buf, size_t len,
		int flags, sockaddr_in* addr, socklen_t* addrlen) {
	MUDDY_ASSURE_NATURAL(recvfrom(sock, buf, len, flags,
			reinterpret_cast<sockaddr*>(addr), addrlen));
}

inline ssize_t sendto_(SocketHandle sock, const void* buf, size_t len,
		int flags, const sockaddr_in* addr, socklen_t addrlen) {
	MUDDY_ASSURE_NATURAL(sendto(sock, buf, len, flags,
			reinterpret_cast<const sockaddr*>(addr), addrlen));
}

}} // namespce muddy::wrapper
