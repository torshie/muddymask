#ifndef MUDDY_COMMON_INET_DATAGRAM_SOCKET_WIN32_HPP_INCLUDED_
#define MUDDY_COMMON_INET_DATAGRAM_SOCKET_WIN32_HPP_INCLUDED_

namespace muddy {

class DatagramSocket {
public:
	DatagramSocket();
	~DatagramSocket();

	void bind(const sockaddr_in& address);
	int read(void* buffer, int size, sockaddr_in* client);
	int write(const void* buffer, int size, const sockaddr_in& remote);

private:
	SOCKET sock;
};

} // namespace muddy

#endif // MUDDY_COMMON_INET_DATAGRAM_SOCKET_WIN32_HPP_INCLUDED_
