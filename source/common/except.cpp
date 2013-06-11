#include <cstdarg>
#include <exception>
#include <config.h>
#include <muddy/common/compat/memory>
#include <muddy/common/except.hpp>

using namespace muddy;

IgnoredExcept::IgnoredExcept()
		: message(static_cast<char*>(malloc(MAX_MESSAGE_LENGTH)),
				&std::free) {
	*message = '\0';
}

void IgnoredExcept::init(const char* format, va_list ap) {
	char* buffer = message.get();
	int length = std::strlen(buffer);
	if (format != NULL && length < MAX_MESSAGE_LENGTH - 1) {
		buffer[length++] = ' ';
		vsnprintf(buffer + length, MAX_MESSAGE_LENGTH - length, format,
				ap);
	}
}
