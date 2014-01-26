#include <cstdio>
#include <type_traits>
#include <muddy/common/except.hpp>
#include <muddy/common/util/define.hpp>
#include <muddy/common/mm/ArenaAllocator.hpp>

using namespace muddy;

struct s {
	MUDDY_ARENA_ALLOC(s);
};

void f() {
	throw LostOurWay{};
}

int main()
try {
	f();
	return 0;
} catch (IgnoredExcept& e) {
	std::printf("%s\n", e.what());
	return 1;
}
