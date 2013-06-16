#include <muddy/common/except.hpp>
#include <muddy/common/InitManager.hpp>
#include <muddy/common/thread/Thread.hpp>

using namespace muddy;

Thread::Thread(void (*e)(void*), void* a) : entry(e), argument(a) {
	int r = pthread_create(&thread, NULL, threadEntry, this);
	if (r != 0) {
		throw SystemError(r);
	}
}

void Thread::wait() {
	void* v;
	int r = pthread_join(thread, &v);
	if (r != 0) {
		throw SystemError(r);
	}
}

void* Thread::threadEntry(void* arg) {
	Thread* self = static_cast<Thread*>(arg);

	InitManager::initThread();
	pthread_cleanup_push(&InitManager::leaveThread, NULL);
	self->entry(self->argument);
	pthread_cleanup_pop(1);
	return NULL;
}
