namespace muddy { namespace wrapper {

void bind_(int s, const void* addr, socklen_t len) {
	if (bind(s, static_cast<const sockaddr*>(addr), len) != 0) {
		throw SystemError{WSAGetLastError()};
	}
}

}} // namespce muddy::wrapper
