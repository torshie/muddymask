#ifndef MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_
#define MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_

#include <muddy/common/logging/Logger.hpp>

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
	typedef typename helper::FirstType<(tLevel >= LOGGING_LEVEL), Logger,
			Dummy>::T T;
};

template<Severity tLevel>
typename LoggerType<tLevel>::T log() {
	typedef typename LoggerType<tLevel>::T T;
	return T(tLevel);
}

#define LOG_TRACE ::muddy::logging::log< ::muddy::logging::kTrace>()
#define LOG_DEBUG ::muddy::logging::log< ::muddy::logging::kDebug>()
#define LOG_VERBOSE ::muddy::logging::log< ::muddy::logging::kVerbose>()
#define LOG_INFO ::muddy::logging::log< ::muddy::logging::kInfo>()
#define LOG_NOTICE ::muddy::logging::log< ::muddy::logging::kNotice>()
#define LOG_WARNING ::muddy::logging::log< ::muddy::logging::kWarning>()
#define LOG_ERROR ::muddy::logging::log< ::muddy::logging::kError>()
#define LOG_FATAL ::muddy::logging::log< ::muddy::logging::kFatal>()

}} // namespace muddy::logging

#endif // MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_
