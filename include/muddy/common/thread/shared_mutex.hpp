#ifndef MUDDY_COMMON_THREAD_SHARED_MUTEX_HPP_INCLUDED_
#define MUDDY_COMMON_THREAD_SHARED_MUTEX_HPP_INCLUDED_

#include <condition_variable>
#include <mutex>

namespace muddy {

// TODO The implementation is copied from libc++
class shared_mutex {
public:
	shared_mutex();

	void lock();
	bool try_lock();
	void unlock();

	void lock_shared();
	bool try_lock_shared();
	void unlock_shared();

private:
	std::mutex __mut_;
	std::condition_variable __gate1_;
	std::condition_variable __gate2_;
	unsigned __state_;

	static const unsigned __write_entered_ =
			1U << (sizeof(unsigned) * 8 - 1);
	static const unsigned __n_readers_ = ~__write_entered_;
};

} // namespace muddy

#endif // MUDDY_COMMON_THREAD_SHARED_MUTEX_HPP_INCLUDED_
