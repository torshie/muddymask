#if HAVE_SOCK_UNIX
#	define MUDDY_ASSURE_SOCKET(...) \
		::muddy::inner::wrapper::assureNatural(__VA_ARGS__)
#elif HAVE_SOCK_WINSOCK2
#	define MUDDY_ASSURE_SOCKET(...) \
		::muddy::inner::wrapper::assureValidWinsock(__VA_ARGS__)
#else
#	error "Your platform isn't supported."
#endif

namespace muddy { namespace wrapper {

inline SocketHandle socket_(int af, int type, int proto) {
	return MUDDY_ASSURE_SOCKET(socket(af, type, proto));
}

inline in_addr_t inet_addr_(const char* str) {
	int a, b, c, d;
	if (std::sscanf(str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4
			|| a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255
			|| d < 0 || d > 255) {
		throw InvalidArgument("Invalid IP address: ", str);
	}
	return htonl(a << 24 | b << 16 | c << 8 | d);
}

}} // namespace muddy::wrapper

#undef MUDDY_ASSURE_SOCKET
