#include <cerrno>
#include <cstdio>

namespace muddy { namespace wrapper {

inline void fclose_(FILE* file) {
	MUDDY_ASSURE_ZERO(std::fclose(file));
}

inline FILE* fopen_(const char* name, const char* mode) {
	FILE* file = std::fopen(name, mode);
	if (file == nullptr) {
		throw SystemError{errno, name};
	}
	return file;
}

inline size_t fwrite_(const void *ptr, size_t size, size_t nmemb,
		FILE* stream) {
	size_t r = std::fwrite(ptr, size, nmemb, stream);
	if (r < nmemb) {
		if (std::ferror(stream)) {
			throw SystemError{errno};
		} else {
			throw LostOurWay{};
		}
	}
	return r;
}

}} // namespace muddy::wrapper
