#include <cstring>
#include <muddy/crypto/DummyCrypto.hpp>

using namespace muddy;

DummyCrypto::DummyCrypto(const char* str) : key(0) {
	int length = std::strlen(str);
	int i;
	for (i = 0; i < length / 4 * 4; i += 4) {
		key ^= *reinterpret_cast<const uint32_t*>(str + i);
	}
	if (i < length) {
		uint32_t v = 0;
		std::memcpy(&v, str + i, length - i);
		key ^= v;
	}
}

void DummyCrypto::encrypt(void* t, int length) {
	// TODO handle unaligned addresses.
	char* text = static_cast<char*>(t);
	int i;
	for (i = 0; i < length / 4 * 4; i += 4) {
		*reinterpret_cast<uint32_t*>(text + i) ^= key;
	}
	if (i < length) {
		uint32_t v = 0;
		std::memcpy(&v, text + i, length - i);
		v ^= key;
		std::memcpy(text + i, &v, length - i);
	}
}

void DummyCrypto::decrypt(void* cypher, int length) {
	encrypt(cypher, length);
}
