#include <muddy/common/thread/shared_mutex.hpp>

using namespace muddy;

shared_mutex::shared_mutex() : __state_(0) {}

void shared_mutex::lock() {
	std::unique_lock<std::mutex> lk(__mut_);
	while (__state_ & __write_entered_) {
		__gate1_.wait(lk);
	}
	__state_ |= __write_entered_;
	while (__state_ & __n_readers_) {
		__gate2_.wait(lk);
	}
}

bool shared_mutex::try_lock() {
	std::unique_lock<std::mutex> lk(__mut_);
	if (__state_ == 0) {
		__state_ = __write_entered_;
		return true;
	}
	return false;
}

void shared_mutex::unlock() {
	std::lock_guard<std::mutex> _(__mut_);
	__state_ = 0;
	__gate1_.notify_all();
}

void shared_mutex::lock_shared() {
	std::unique_lock<std::mutex> lk(__mut_);
	while ((__state_ & __write_entered_)
			|| (__state_ & __n_readers_) == __n_readers_) {
		__gate1_.wait(lk);
	}
	unsigned num_readers = (__state_ & __n_readers_) + 1;
	__state_ &= ~__n_readers_;
	__state_ |= num_readers;
}

bool shared_mutex::try_lock_shared() {
	std::unique_lock<std::mutex> lk(__mut_);
	unsigned num_readers = __state_ & __n_readers_;
	if (!(__state_ & __write_entered_) && num_readers != __n_readers_) {
		++num_readers;
		__state_ &= ~__n_readers_;
		__state_ |= num_readers;
		return true;
	}
	return false;
}

void shared_mutex::unlock_shared() {
	std::lock_guard<std::mutex> _(__mut_);
	unsigned num_readers = (__state_ & __n_readers_) - 1;
	__state_ &= ~__n_readers_;
	__state_ |= num_readers;
	if (__state_ & __write_entered_) {
		if (num_readers == 0) {
			__gate2_.notify_one();
		}
	} else {
		if (num_readers == __n_readers_ - 1) {
			__gate1_.notify_one();
		}
	}
}
