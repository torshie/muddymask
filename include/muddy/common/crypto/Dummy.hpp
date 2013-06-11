#ifndef MUDDY_COMMON_CRYPTO_DUMMY_HPP_INCLUDED_
#define MUDDY_COMMON_CRYPTO_DUMMY_HPP_INCLUDED_

#include <string>
#include <muddy/common/compat/stdint.h>

namespace muddy { namespace crypto {

class Dummy {
public:
	explicit Dummy(uint64_t k) : key(k) {}

	void encrypt(const std::string& text, std::string* output);
	void decrypt(const std::string& data, std::string* text);

	void setKey(uint64_t k) {
		key = k;
	}

private:
	uint64_t key;
};

}} // muddy

#endif // MUDDY_COMMON_CRYPTO_DUMMY_HPP_INCLUDED_
