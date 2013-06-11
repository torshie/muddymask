#include <muddy/common/helper.hpp>
#include <muddy/common/crypto/Dummy.hpp>

using namespace muddy;
using namespace muddy::crypto;

void Dummy::encrypt(const std::string& text, std::string* output) {
	typedef helper::IntegerType<sizeof(key)>::U ChunkType;
	const char* end = text.c_str() + text.size();
	for (const char* p = text.c_str(); p < end; p += sizeof(key)) {
		ChunkType chunk = *reinterpret_cast<const ChunkType*>(p);
		chunk ^= key;
		output->append(reinterpret_cast<char*>(&chunk), sizeof(chunk));
	}
	if (text.size() % sizeof(key) != 0) {
		ChunkType chunk = 0;
		size_t remaining = text.size() % sizeof(key);
		std::memcpy(&chunk, end - remaining, remaining);
		output->append(reinterpret_cast<char*>(&chunk), remaining);
	}
}

void Dummy::decrypt(const std::string& data, std::string* text) {
	// It simply works!
	encrypt(data, text);
}
