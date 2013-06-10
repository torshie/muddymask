#ifndef MUDDY_COMMON_EXCEPT_HPP_INCLUDED_
#define MUDDY_COMMON_EXCEPT_HPP_INCLUDED_

#include <cstddef>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <exception>
#include <muddy/compat/memory>

namespace muddy {

class IgnoredExcept : public std::exception {
public:
	virtual ~IgnoredExcept() throw() {}

	virtual const char* what() const throw() {
		return message.get();
	}

protected:
	enum { MAX_MESSAGE_LENGTH = 1024 };

	shared_ptr<char> message;

	IgnoredExcept();

	void init(const char* format, va_list ap);
};

#define BUILD_MESSAGE_BUFFER(last) \
	do { \
		va_list ap; \
		va_start(ap, last); \
		this->init(last, ap); \
		va_end(ap); \
	} while(false)

class BufferOverflow : public IgnoredExcept {
public:
	BufferOverflow(const char* format = NULL, ...) {
		BUILD_MESSAGE_BUFFER(format);
	}
};

class LineTooLong : public IgnoredExcept {
public:
	LineTooLong(const char* format = NULL, ...) {
		BUILD_MESSAGE_BUFFER(format);
	}
};

class ConfigError : public IgnoredExcept {
public:
	ConfigError(const char* format = NULL, ...) {
		BUILD_MESSAGE_BUFFER(format);
	}
};

class InvalidArgument : public IgnoredExcept {
public:
	InvalidArgument(const char* format = NULL, ...) {
		BUILD_MESSAGE_BUFFER(format);
	}
};

class InvalidInetAddress : public IgnoredExcept {
public:
	InvalidInetAddress(const char* cp, const char* format = NULL, ...) {
		BUILD_MESSAGE_BUFFER(format);
		int length = std::strlen(message.get());
		std::sprintf(message.get() + length, ", %s", cp);
	}
};

class SystemError : public IgnoredExcept {
public:
	SystemError(int errorCode, const char* format = NULL, ...) {
		BUILD_MESSAGE_BUFFER(format);
		int length = std::strlen(message.get());
		std::sprintf(message.get() + length, ", %s",
				std::strerror(errorCode));
	}
};

class Unpossible : public IgnoredExcept {
public:
	Unpossible(const char* format = NULL, ...) {
		BUILD_MESSAGE_BUFFER(format);
	}
};

#undef BUILD_MESSAGE_BUFFER

} // namespace muddy

#endif // MUDDY_COMMON_EXCEPT_HPP_INCLUDED_
