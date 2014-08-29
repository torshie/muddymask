#ifndef MUDDY_CRYPTO_DUMMY_CRYPTO_HPP_INCLUDED_
#define MUDDY_CRYPTO_DUMMY_CRYPTO_HPP_INCLUDED_

#include <cstdint>

namespace muddy {

class DummyCrypto {
public:
	DummyCrypto(const char* str);

	void encrypt(void* cypher, int length);
	void decrypt(void* text, int length);

private:
	uint32_t key;
};

} // namespace muddy

#endif // MUDDY_CRYPTO_DUMMY_CRYPTO_HPP_INCLUDED_
