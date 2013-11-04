#ifndef MUDDY_COMMON_CRYPTO_DUMMY_HPP_INCLUDED_
#define MUDDY_COMMON_CRYPTO_DUMMY_HPP_INCLUDED_

#include <string>
#include <muddy/common/compat/stdint.h>

namespace muddy { namespace crypto {

class Dummy {
public:
	typedef uint64_t CryptoKey;

	explicit Dummy(CryptoKey k) : key(k) {}

	void encrypt(const void* text, int length, std::string* output) const;
	void decrypt(const void* data, int length, std::string* text) const;

	void setKey(CryptoKey k) {
		key = k;
	}

private:
	CryptoKey key;
};

}} // muddy

#endif // MUDDY_COMMON_CRYPTO_DUMMY_HPP_INCLUDED_
