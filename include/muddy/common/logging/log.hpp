#ifndef MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_
#define MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_

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

#define TRACE ::muddy::logging::log< ::search::logging::kTrace>()
#define DEBUG ::muddy::logging::log< ::search::logging::kDebug>()
#define VERBOSE ::muddy::logging::log< ::search::logging::kVerbose>()
#define INFO ::muddy::logging::log< ::search::logging::kInfo>()
#define NOTICE ::muddy::logging::log< ::search::logging::kNotice>()
#define WARNING ::muddy::logging::log< ::search::logging::kWarning>()
#define ERROR ::muddy::logging::log< ::search::logging::kError>()
#define FATAL ::muddy::logging::log< ::search::logging::kFatal>()

}} // namespace muddy::logging

#endif // MUDDY_COMMON_LOGGING_LOG_HPP_INCLUDED_
