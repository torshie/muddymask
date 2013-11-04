#ifndef MUDDY_COMMON_CRYPTO_HASH_HPP_INCLUDED_
#define MUDDY_COMMON_CRYPTO_HASH_HPP_INCLUDED_

#include <muddy/common/compat/stdint.h>

namespace muddy { namespace crypto {

struct MurmurHash {
	static uint64_t hash(const void* data, int size) {
		uint64_t v[2];
		hash(data, size, 0, v);
		return v[0] ^ v[1];
	}

	static void hash(const void* data, int size, uint32_t seed,
			void* output);
};

}} // namespace muddy::crypto

#endif // MUDDY_COMMON_CRYPTO_HASH_HPP_INCLUDED_
