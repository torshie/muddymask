#ifndef MUDDY_COMMON_INIT_GLOBAL_HPP_INCLUDED_
#define MUDDY_COMMON_INIT_GLOBAL_HPP_INCLUDED_

#include <new>

namespace muddy {

template<typename T>
class InitGlobal {
public:
	static T& get() {
		return T::get();
	}

	InitGlobal() {
		if (counter++ == 0) {
			new (&T::get()) T();
		}
	}

	~InitGlobal() {
		if (--counter == 0) {
			T::get().~T();
		}
	}

private:
	static int counter;
};

template<typename T>
int InitGlobal<T>::counter = 0;

} // namespace muddy

#endif // MUDDY_COMMON_INIT_GLOBAL_HPP_INCLUDED_
