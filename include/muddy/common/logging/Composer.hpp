#ifndef MUDDY_COMMON_LOGGING_COMPOSER_HPP_INCLUDED_
#define MUDDY_COMMON_LOGGING_COMPOSER_HPP_INCLUDED_

#include <sstream>
#include <string>
#include <muddy/common/wrapper.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/logging/Logger.hpp>

namespace muddy { namespace logging {

class Composer {
public:
	Composer(Severity level = MUDDY_LOGGING_LEVEL)
			: logger(Logger::get()), buffer(logger.getBuffer()),
			level(level), continuous(false) {
		blackhole = (level < MUDDY_LOGGING_LEVEL
				|| level < Logger::get().getLevel());
	}

	~Composer() {
		if (blackhole || continuous) {
			return;
		}
		logger.commit(level);
	}

	template<typename T>
	const Composer& operator << (const T& data) const {
		if (!blackhole) {
			buffer << data;
		}
		return *this;
	}

	const Composer& operator << (
			const ::muddy::inner::Concatenator& /* c */) const {
		if (!blackhole) {
			const_cast<Composer*>(this)->continuous = true;
		}
		return *this;
	}

private:
	Logger& logger;
	std::ostringstream& buffer;
	Severity level;
	bool continuous;
	bool blackhole;
};

}} // namespace search::logging

#endif // MUDDY_COMMON_LOGGING_COMPOSER_HPP_INCLUDED_
