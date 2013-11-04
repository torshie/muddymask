#include <cstring>
#include <muddy/common/helper.hpp>
#include <muddy/common/crypto/Dummy.hpp>

using namespace muddy;
using namespace muddy::crypto;

void Dummy::encrypt(const void* text, int length,
		std::string* output) const {
	const char* end = static_cast<const char*>(text) + length;
	for (const char* p = static_cast<const char*>(text);
			p < end; p += sizeof(key)) {
		CryptoKey chunk = *reinterpret_cast<const CryptoKey*>(p);
		chunk ^= key;
		output->append(reinterpret_cast<char*>(&chunk), sizeof(chunk));
	}
	if (length % sizeof(key) != 0) {
		CryptoKey chunk = 0;
		int remaining = length % sizeof(key);
		std::memcpy(&chunk, end - remaining, remaining);
		output->append(reinterpret_cast<char*>(&chunk), remaining);
	}
}

void Dummy::decrypt(const void* data, int length,
		std::string* text) const {
	// It simply works!
	encrypt(data, length, text);
}
