#ifndef MUDDY_COMMON_WRAPPER_LIBC_HPP_INCLUDED_
#define MUDDY_COMMON_WRAPPER_LIBC_HPP_INCLUDED_

#ifndef MUDDY_COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <muddy/common/wrapper/wrapper.hpp> instead"
#endif

#include <cerrno>
#include <cstdio>

namespace muddy { namespace wrapper {

inline void fclose_(FILE* file) {
	CHECK_RETURN_ZERO(std::fclose(file));
}

inline FILE* fopen_(const char* name, const char* mode) {
	FILE* file = std::fopen(name, mode);
	if (file == NULL) {
		throw SystemError(errno, "%s", name);
	}
	return file;
}

inline size_t fwrite_(const void* buffer, size_t size, size_t count,
		FILE* file) {
	size_t ret = std::fwrite(buffer, size, count, file);
	if (ret != count) {
		throw SystemError(errno);
	}
	return ret;
}

inline size_t fread_(void *ptr, size_t size, size_t count, FILE *stream) {
	size_t ret = std::fread(ptr, size, count, stream);
	if (ret < count && std::ferror(stream)) {
		throw SystemError(errno);
	}
	return ret;
}

inline int fgetc_(FILE* file) {
	int ch = std::fgetc(file);
	if (ch == EOF && std::ferror(file)) {
		throw SystemError(errno);
	}
	return ch;
}

inline void fputc_(int ch, FILE* file) {
	int r = std::fputc(ch, file);
	if (r == EOF) {
		throw SystemError(errno);
	}
}

inline void fflush_(FILE* file) {
	CHECK_RETURN_ZERO(std::fflush(file));
}

inline void fseek_(FILE *stream, long offset, int whence) {
	CHECK_RETURN_ZERO(std::fseek(stream, offset, whence));
}

inline void fprintf_(FILE* file, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int ret = std::vfprintf(file, format, ap);
	va_end(ap);
	if (ret < 0) {
		throw SystemError(errno);
	}
}

inline void fputs_(const char* string, FILE* file) {
	int ret = std::fputs(string, file);
	if (ret == EOF) {
		throw SystemError(errno);
	}
}

inline void atexit_(void (*func)()) {
	CHECK_RETURN_ZERO(std::atexit(func));
}

}} // namespace muddy::wrapper

#endif // MUDDY_COMMON_WRAPPER_LIBC_HPP_INCLUDED_
