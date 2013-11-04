#ifndef MUDDY_COMMON_THREAD_SYNC_WIN32_HPP_INCLUDED_
#define MUDDY_COMMON_THREAD_SYNC_WIN32_HPP_INCLUDED_

#include <windows.h>

namespace muddy {

class ThreadMutex {
	friend class Guard;
	friend class ReadWriteLock;
public:
	ThreadMutex() {
		InitializeCriticalSection(&cs);
	}

	~ThreadMutex() {
		DeleteCriticalSection(&cs);
	}

private:
	CRITICAL_SECTION cs;

	void lock() {
		EnterCriticalSection(&cs);
	}

	void unlock() {
		LeaveCriticalSection(&cs);
	}
};

class ReadWriteLock {
public:
	ReadWriteLock() : readerCount(0) {
		noReader = CreateEvent (NULL, TRUE, TRUE, NULL);
	}

	void lockRead();
	void unlockRead();

	void lockWrite() {
		writeMutex.lock();
		WaitForSingleObject(noReader, INFINITE);
	}

	void unlockWrite() {
		writeMutex.unlock();
	}

private:
	volatile int readerCount;
	HANDLE noReader;
	ThreadMutex writeMutex;
	ThreadMutex counterMutex;
};

} // namespace muddy

#endif // MUDDY_COMMON_THREAD_SYNC_WIN32_HPP_INCLUDED_
