#ifndef MUDDY_COMMON_STACK_TRACE_HPP_
#define MUDDY_COMMON_STACK_TRACE_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace muddy {

class StackTrace {
	struct Symbol {
		uintptr_t address;
		std::string file;
		int line;
		std::string name;
	};
public:
	explicit StackTrace(int skip = 1);

	std::vector<Symbol>::const_iterator begin() const {
		return trace.begin();
	}

	std::vector<Symbol>::const_iterator end() const {
		return trace.end();
	}

private:
	std::vector<Symbol> trace;

#if !_WIN32
	static int append(void* data, uintptr_t pc, const char* filename,
			int lineno, const char* function);
#endif
};

} // namespace muddy

#endif // MUDDY_COMMON_STACK_TRACE_HPP_
