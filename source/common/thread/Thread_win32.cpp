#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <muddy/common/compat/memory>
#include <muddy/common/except.hpp>
#include <muddy/common/InitManager.hpp>
#include <muddy/common/thread/Thread_win32.hpp>

using namespace muddy;

Thread::Thread(void (*e)(void*), void* a) : entry(e), argument(a) {
	thread = _beginthreadex(NULL, 0, &call, this, 0, NULL);
	if (thread == 0) {
		throw Unpossible();
	}
}

Thread::~Thread() {
	if (CloseHandle(reinterpret_cast<HANDLE>(thread)) != 0) {
		throw Unpossible();
	}
}

void Thread::wait() {
	if (WaitForSingleObject(reinterpret_cast<HANDLE>(thread), INFINITE)
			!= 0) {
		throw Unpossible();
	}
}

unsigned __stdcall Thread::call(void* p) {
	Thread* self = static_cast<Thread*>(p);

	InitManager::initThread();
	try {
		self->entry(self->argument);
	} catch (...) {
		// TODO leaveThread() won't be called if the thread is canceled.
		InitManager::leaveThread(NULL);
		throw;
	}
	InitManager::leaveThread(NULL);
	return 0;
}
