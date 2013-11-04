#ifndef MUDDY_TUNNEL_CRYPTOGRAPHER_HPP_INCLUDED_
#define MUDDY_TUNNEL_CRYPTOGRAPHER_HPP_INCLUDED_

#include <muddy/common/crypto/Dummy.hpp>

namespace muddy {

class Cryptographer {
public:
	explicit Cryptographer(const char* secret);

	void encrypt(const char* text, int length, std::string* output);
	void decrypt(const char* data, int length, std::string* text);

private:
	enum { kTimeFrame = 30 };
	typedef crypto::Dummy::CryptoKey CryptoKey;

	const static uint32_t kCryptoMagic = 0xd119b00b;

	time_t keyUpdateTime;
	CryptoKey baseKey;
	CryptoKey lastKey, currentKey, nextKey;
	crypto::Dummy crypto;

	void updateCryptoKey();
};

} // namespace muddy

#endif // MUDDY_TUNNEL_CRYPTOGRAPHER_HPP_INCLUDED_
