#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <cxxabi.h>
#include <backtrace/backtrace.h>
#include <muddy/common/util/path.hpp>
#include <muddy/common/StackTrace.hpp>

using namespace muddy;

StackTrace::StackTrace(int skip) {
	backtrace_state* state = backtrace_create_state(nullptr, 0, nullptr,
			nullptr);
	void (*empty)(void*, const char*, int) =
			[](void*, const char*, int) {};
	backtrace_full(state, skip, &append, empty, this);
}

int StackTrace::append(void* data, uintptr_t pc, const char* filename,
		int lineno, const char* function) {
	auto trace = static_cast<StackTrace*>(data);
	if (filename != nullptr) {
		filename = util::basename(filename);
	} else {
		filename = "???";
		lineno = 0;
	}
	if (function != nullptr) {
		int status;
		char* demangled = abi::__cxa_demangle(function, nullptr, nullptr,
				&status);
		const char* symbol = (status == 0) ? demangled : function;
		trace->trace.push_back(Symbol{pc, filename, lineno, symbol});
		std::free(demangled);
	} else {
		trace->trace.push_back(Symbol{pc, filename, lineno, "???"});
	}
	return 0;
}
