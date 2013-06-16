#ifndef MUDDY_COMMON_LOGGING_LOGGER_HPP_INCLUDED_
#define MUDDY_COMMON_LOGGING_LOGGER_HPP_INCLUDED_

#include <sstream>
#include <muddy/common/helper.hpp>

namespace muddy { namespace logging {

enum Severity {
	kTrace = 1, kDebug, kVerbose, kInfo, kNotice, kWarning, kError, kFatal,
	kSeverityLevelCount = kFatal
};

#ifndef LOGGING_LEVEL
#	if ENABLE_DEBUGGING_CODE
#		define LOGGING_LEVEL kDebug
#	else
#		define LOGGING_LEVEL kVerbose
#	endif
#endif

class Logger {
public:
	explicit Logger(Severity level = LOGGING_LEVEL)
			: level(level), stream(NULL) {
		if (level >= LOGGING_LEVEL && level >= loggingLevel) {
			stream = new std::ostringstream();
		}
	}

	~Logger();

	static void setLevel(Severity level) {
		loggingLevel = level;
	}

	template<typename T>
	const Logger& operator << (const T& data) const {
		if (stream != NULL) {
			*stream << data;
		}
		return *this;
	}

private:
	static Severity loggingLevel;

	Severity level;
	std::ostringstream* stream;
};

}} // namespace muddy::logging

#endif // MUDDY_COMMON_LOGGING_LOGGER_HPP_INCLUDED_
