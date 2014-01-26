#include <iomanip>
#include <sstream>
#include <utility>
#include <muddy/common/except.hpp>

using namespace muddy;

std::string IgnoredExcept::getStackTrace(const StackTrace& trace) {
	std::ostringstream stream;
	for (const auto& i : trace) {
		stream << "    " << std::setw(sizeof(i.address) * 2)
				<< std::setfill('0') << std::hex << i.address
				<< std::dec << ' '
				<< i.file;
		if (i.line != 0) {
			stream << ':' << i.line;
		}
		stream << ' ' << i.name << '\n';
	}
	return std::move(stream.str());
}
