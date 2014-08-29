#ifndef MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_
#define MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_

#include <type_traits>
#include <muddy/common/logging/misc.hpp>
#include <muddy/common/logging/Composer.hpp>

namespace muddy { namespace logging {

struct Dummy {
	Dummy(Severity level) {
		(void)level;
	}

	template<typename T>
	Dummy& operator << (const T& /* unused */) {
		return *this;
	}
};

template<Severity tLevel>
struct LoggerType {
	typedef typename std::conditional<(tLevel >= MUDDY_LOGGING_LEVEL),
			Composer, Dummy>::type T;
};

template<Severity tLevel>
typename LoggerType<tLevel>::T log() {
	typedef typename LoggerType<tLevel>::T T;
	return T(tLevel);
}

#define MUDDY_TRACE ::muddy::logging::log< ::muddy::logging::kTrace>()
#define MUDDY_DEBUG ::muddy::logging::log< ::muddy::logging::kDebug>()
#define MUDDY_VERBOSE ::muddy::logging::log< ::muddy::logging::kVerbose>()
#define MUDDY_INFO ::muddy::logging::log< ::muddy::logging::kInfo>()
#define MUDDY_NOTICE ::muddy::logging::log< ::muddy::logging::kNotice>()
#define MUDDY_WARNING ::muddy::logging::log< ::muddy::logging::kWarning>()
#define MUDDY_ERROR ::muddy::logging::log< ::muddy::logging::kError>()
#define MUDDY_FATAL ::muddy::logging::log< ::muddy::logging::kFatal>()

}} // namespace muddy::logging

#endif // MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_
