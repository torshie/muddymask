#include <muddy/common/wrapper.hpp>
#include <muddy/common/logging/log.hpp>
#include <muddy/common/util/define.hpp>
#include <muddy/common/StackTrace.hpp>

using namespace muddy;

StackTrace::StackTrace(int skip) {
	void* address[200];
	auto n = CaptureStackBackTrace(skip, MUDDY_ARRAY_SIZE(address),
			address, nullptr);
	for (decltype(n) i = 0; i < n; ++i) {
		trace.push_back(
				Symbol{reinterpret_cast<uintptr_t>(address[i]),
			"???", 0, "???"});
	}
}
