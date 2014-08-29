
#ifndef MUDDY_COMMON_TPL_SHARED_LOCK_HPP_INCLUDED_
#define MUDDY_COMMON_TPL_SHARED_LOCK_HPP_INCLUDED_

namespace muddy { namespace tpl {

template<typename T>
class shared_lock {
public:
	shared_lock(T& m) : mutex(m) {
		mutex.lock_shared();
	}

	~shared_lock() {
		mutex.unlock_shared();
	}

private:
	T& mutex;
};

}} // namespace muddy::tpl

#endif // MUDDY_COMMON_TPL_SHARED_LOCK_HPP_INCLUDED_
