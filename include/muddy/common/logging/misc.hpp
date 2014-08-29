#ifndef MUDDY_COMMON_LOGGING_MISC_HPP_INCLUDED_
#define MUDDY_COMMON_LOGGING_MISC_HPP_INCLUDED_

namespace muddy { namespace inner {

class Concatenator {};

}} // namespace muddy::inner;

namespace muddy { namespace logging {

#ifndef MUDDY_LOGGING_LEVEL
#	if MUDDY_ENABLE_DEBUGGING
#		define MUDDY_LOGGING_LEVEL kDebug
#	else
#		define MUDDY_LOGGING_LEVEL kVerbose
#	endif
#endif

enum Severity {
	kTrace = 1, kDebug, kVerbose, kInfo, kNotice, kWarning, kError, kFatal,
	kSeverityLevelCount = kFatal
};

extern const char* const kLevelName[kSeverityLevelCount];
extern const inner::Concatenator continuous;

}} // namespace muddy::logging

#endif // MUDDY_COMMON_LOGGING_MISC_HPP_INCLUDED_
