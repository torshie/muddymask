#include <muddy/common/wrapper.hpp>
#include <muddy/net/StartWSA.hpp>

using namespace muddy;

StartWSA& StartWSA::get() {
	alignas(StartWSA) static char storage[sizeof(StartWSA)];
	return reinterpret_cast<StartWSA&>(storage);
}
