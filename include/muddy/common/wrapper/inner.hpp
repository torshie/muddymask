namespace muddy { namespace inner { namespace wrapper {

template<typename Ret, typename... Args>
void assureZero(Ret v, Args... args) {
	if (MUDDY_UNLIKELY(v != 0)) {
		throw SystemError(errno, args...);
	}
}

template<typename Ret, typename... Args>
Ret assureNatural(Ret v, Args... args) {
	if (MUDDY_UNLIKELY(v < 0)) {
		throw SystemError(errno, args...);
	}
	return v;
}

#if HAVE_SYSCALL_WIN32

template<typename Ret, typename... Args>
void assureTrue(Ret v, Args... args) {
	if (MUDDY_UNLIKELY(v == 0)) {
		// Note parenthesises are used to explicitly allow narrowing.
		throw SystemError(GetLastError(), args...);
	}
}

#endif // #if HAVE_SYSCALL_WIN32

#if HAVE_SOCK_WINSOCK2

template<typename... Args>
SocketHandle assureValidWinsock(SocketHandle h, Args... args) {
	if (MUDDY_UNLIKELY(h == INVALID_SOCKET)) {
		throw SystemError{WSAGetLastError(), args...};
	}
	return h;
}

#endif // #if HAVE_SOCK_WINSOCK2

}}} // namespace muddy::inner::wrapper
