#ifndef MUDDY_COMMON_CRYPTO_HASH_HPP_INCLUDED_
#define MUDDY_COMMON_CRYPTO_HASH_HPP_INCLUDED_

#include <muddy/common/compat/stdint.h>

namespace muddy { namespace crypto {

struct MurmurHash {
	static void hash(const void* data, int size, uint32_t seed,
			void* output);
};

}} // namespace muddy::crypto

#endif // MUDDY_COMMON_CRYPTO_HASH_HPP_INCLUDED_
