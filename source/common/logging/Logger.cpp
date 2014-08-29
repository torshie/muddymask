#include <cstdio>
#include <cstring>
#include <ctime>
#include <thread>
#include <type_traits>
#include <muddy/common/wrapper.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/logging/Logger.hpp>

namespace muddy { namespace logging {

const char* const kLevelName[kSeverityLevelCount] = {
	"[TRACE]", "[DEBUG]", "[VERBOSE]", "[INFO]", "[NOTICE]", "[WARNING]",
	"[ERROR]", "[FATAL]"
};

const inner::Concatenator continuous{};

thread_local std::ostringstream Logger::buffer;
thread_local std::string Logger::identity;

Logger& Logger::get() {
	alignas(Logger) static char storage[sizeof(Logger)];
	return reinterpret_cast<Logger&>(storage);
}

void Logger::commit(Severity l) {
	char prefix[1024];
	int length = getLogPrefix(prefix, sizeof(prefix), l);
	buffer << "\n";
	std::string tmp = std::move(buffer.str());
	{
		FILE* f = (logFile == nullptr) ? stderr : logFile;
		std::lock_guard<std::mutex> guard{mutex};
		wrapper::fwrite_(prefix, 1, length, f);
		wrapper::fwrite_(tmp.c_str(), 1, tmp.size(), f);
	}
	buffer.str("");
}

void Logger::setTarget(const char* path) {
	if (path == nullptr || path[0] == '\0') {
		return;
	}
	FILE* f = wrapper::fopen_(path, "a");
	{
		std::lock_guard<std::mutex> guard{mutex};
		if (logFile != nullptr) {
			wrapper::fclose_(logFile);
		}
		logFile = f;
	}
}

int Logger::getLogPrefix(char* buffer, int size, Severity l) {
	if (identity.empty()) {
		std::ostringstream ss;
		ss << std::hex << std::this_thread::get_id();
		setIdentity(ss.str().c_str());
	}

	time_t stamp = time(NULL);
	struct tm now;
	localtime_r(&stamp, &now);
	int i = std::strftime(buffer, size, "[%F %T]", &now);
	buffer[i++] = ' ';

	std::memcpy(buffer + i, identity.c_str(), identity.size());
	i += identity.size();

	buffer[i++] = ' ';

	int s = std::strlen(kLevelName[l - 1]);
	std::memcpy(buffer + i, kLevelName[l - 1], s);
	i += s;
	buffer[i++] = ' ';
	return i;
}

}} // namespace muddy::logging
