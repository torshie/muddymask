#ifndef MUDDY_COMMON_LOGGING_LOGGER_HPP_INCLUDED_
#define MUDDY_COMMON_LOGGING_LOGGER_HPP_INCLUDED_

#include <mutex>
#include <string>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/wrapper.hpp>
#include <muddy/common/InitGlobal.hpp>

namespace muddy { namespace logging {

class Logger {
	friend class InitGlobal<Logger>;
public:
	static Logger& get();

	void commit(Severity l);
	void setTarget(const char* target);

	std::ostringstream& getBuffer() {
		return buffer;
	}

	void setIdentity(const char* id) {
		identity.assign(std::string("[") + id + "]");
	}

	Severity getLevel() const {
		return level;
	}

	void setLevel(Severity level) {
		this->level = level;
	}

private:
	static thread_local std::ostringstream buffer;
	static thread_local std::string identity;

	Severity level;
	std::mutex mutex;
	FILE* logFile;

	Logger() : level(MUDDY_LOGGING_LEVEL), logFile(nullptr) {}

	~Logger() {
		if (logFile != nullptr) {
			wrapper::fclose_(logFile);
		}
	}

	int getLogPrefix(char* buffer, int size, Severity l);
};

static InitGlobal<Logger> _initGlobalLogger;

}} // namespace muddy::logging

#endif // MUDDY_COMMON_LOGGING_LOGGER_HPP_INCLUDED_
