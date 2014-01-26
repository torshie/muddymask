#if HAVE_SOCK_UNIX
#	define INNER_ASSURE_SOCKET(...) \
		::muddy::inner::wrapper::assureNonnega(__VA_ARGS__)
#elif HAVE_SOCK_WINSOCK2
#	define INNER_ASSURE_SOCKET(...) \
		::muddy::inner::wrapper::assureValidWinsock(__VA_ARGS__)
#else
#	error "Your platform isn't supported."
#endif

namespace muddy { namespace wrapper {

SocketHandle socket_(int af, int type, int proto) {
	return INNER_ASSURE_SOCKET(socket(af, type, proto));
}

}} // namespace muddy::wrapper

#undef INNER_ASSURE_SOCKET
