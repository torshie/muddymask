#ifndef MUDDY_COMMON_THREAD_POSIX_HPP_INCLUDED_
#define MUDDY_COMMON_THREAD_POSIX_HPP_INCLUDED_

#include <pthread.h>

namespace muddy {

class Thread {
public:
	explicit Thread(void (*entry)(void*), void* arg = NULL);
	~Thread();

	void wait();

private:
	pthread_t thread;
	void (*entry)(void*);
	void* argument;

	static void* threadEntry(void* arg);
};

} // namespace muddy

#endif // MUDDY_COMMON_THREAD_POSIX_HPP_INCLUDED_
