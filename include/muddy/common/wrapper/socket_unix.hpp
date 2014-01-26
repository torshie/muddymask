namespace muddy { namespace wrapper {

void bind_(int s, const void* addr, socklen_t len) {
	INNER_ASSURE_ZERO(bind(s, static_cast<const sockaddr*>(addr), len));
}

}} // namespce muddy::wrapper
