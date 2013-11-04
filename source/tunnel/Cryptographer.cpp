#include <ctime>
#include <cstring>
#include <muddy/common/crypto/hash.hpp>
#include <muddy/tunnel/Cryptographer.hpp>

using namespace muddy;
using namespace muddy::crypto;

Cryptographer::Cryptographer(const char* secret) : crypto(0) {
	uint64_t hash[2];
	MurmurHash::hash(secret, std::strlen(secret), 0, hash);
	baseKey = hash[0] ^ hash[1];

	keyUpdateTime = std::time(NULL) / kTimeFrame * kTimeFrame;
	updateCryptoKey();
}

void Cryptographer::encrypt(const char* text, int length,
		std::string* output) {
	time_t currentTime = std::time(NULL);
	if (currentTime - keyUpdateTime >= kTimeFrame) {
		keyUpdateTime = currentTime / kTimeFrame * kTimeFrame;
		updateCryptoKey();
		crypto.setKey(currentKey);
	}

	crypto.encrypt(&kCryptoMagic, sizeof(kCryptoMagic), output);
	crypto.encrypt(text, length, output);
}

void Cryptographer::decrypt(const char* data, int length,
		std::string* text) {
	time_t now = std::time(NULL);
	if (now - keyUpdateTime >= kTimeFrame) {
		keyUpdateTime = now / kTimeFrame * kTimeFrame;
		updateCryptoKey();
		crypto.setKey(currentKey);
	}

	// TODO Hacking implementation details of Dummy::encrypt().
	std::string tmp;
	crypto.decrypt(data, sizeof(kCryptoMagic), &tmp);
	if (*static_cast<const uint32_t*>(tmp.c_str()) == kCryptoMagic) {
		crypto.decrypt(data + sizeof(kCryptoMagic),
				length - sizeof(kCryptoMagic), text);
	} else {
		tmp.clear();
		crypto.setKey(lastKey);
		crypto.decrypt(data, sizeof(kCryptoMagic), &tmp);
		if (*static_cast<const uint32_t*>(tmp.c_str()) == kCryptoMagic) {
			crypto.decrypt(data + sizeof(kCryptoMagic),
					length - sizeof(kCryptoMagic), text);
		} else {
			tmp.clear();
			crypto.setKey(nextKey);
			crypto.decrypt(data, sizeof(kCryptoMagic), &tmp);
			if (*static_cast<const uint32_t*>(tmp.c_str())
					== kCryptoMagic) {
				crypto.decrypt(data + sizeof(kCryptoMagic),
						length - sizeof(kCryptoMagic), text);
			}
		}
	}
	crypto.setKey(currentKey);
}

void Cryptographer::updateCryptoKey() {
	uint64_t hash[2];
	MurmurHash:hash(&keyUpdateTime, sizeof(keyUpdateTime), baseKey, hash);
	currentKey = hash[0] ^ hash[1];

	time_t previous = keyUpdateTime - kTimeFrame;
	MurmurHash::hash(&previous, sizeof(previous), baseKey, hash);
	lastKey = hash[0] ^ hash[1];

	time_t next = keyUpdateTime + kTimeFrame;
	MurmurHash::hash(&next, sizeof(next), baseKey, hash);
	nextKey = hash[0] ^ hash[1];
}
