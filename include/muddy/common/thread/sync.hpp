#ifndef MUDDY_COMMON_THREAD_SYNC_HPP_INCLUDED_
#define MUDDY_COMMON_THREAD_SYNC_HPP_INCLUDED_

#if _WIN32
#	include <muddy/common/thread/sync_win32.hpp>
#else
#	include <muddy/common/thread/sync_posix.hpp>
#endif

namespace muddy {

class Guard {
public:
	Guard(ThreadMutex& m) : mutex(m) {
		mutex.lock();
	}

	~Guard() {
		mutex.unlock();
	}

private:
	ThreadMutex& mutex;
};

class ReadGuard {
public:
	ReadGuard(ReadWriteLock& l) : lock(l) {
		lock.lockRead();
	}

	~ReadGuard() {
		lock.unlockRead();
	}

private:
	ReadWriteLock& lock;
};

class WriteGuard {
public:
	WriteGuard(ReadWriteLock& l) : lock(l) {
		lock.lockWrite();
	}

	~WriteGuard() {
		lock.unlockWrite();
	}

private:
	ReadWriteLock& lock;
};

} // namespace muddy

#endif // MUDDY_COMMON_THREAD_SYNC_HPP_INCLUDED_
