#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <muddy/common/compat/memory>
#include <muddy/common/except.hpp>
#include <muddy/common/InitManager.hpp>
#include <muddy/common/thread/Thread_win32.hpp>

using namespace muddy;

Thread::Thread(void (*entry)(void*), void* arg) {
	void** combined = static_cast<void**>(std::malloc(sizeof(void*) * 2));
	combined[0] = entry;
	combined[1] = arg;

	tid = _beginthreadex(NULL, 0, &call, combined, 0, NULL);
	if (tid == 0) {
		std::free(combined);
		throw Unpossible();
	}
}

Thread::~Thread() {
	if (CloseHandle(reinterpret_cast<HANDLE>(tid)) != 0) {
		throw Unpossible();
	}
}

void Thread::wait() {
	if (WaitForSingleObject(reinterpret_cast<HANDLE>(tid), INFINITE)
			!= 0) {
		throw Unpossible();
	}
}

unsigned __stdcall Thread::call(void* p) {
	shared_ptr<void*> combined(static_cast<void**>(p), &std::free);
	void (*entry)(void*) = static_cast<void (*)(void*)>(combined.get()[0]);
	void* arg = combined.get()[1];

	InitManager::initThread();
	try {
		entry(arg);
	} catch (...) {
		// TODO leaveThread() won't be called if the thread is canceled.
		InitManager::leaveThread(NULL);
		throw;
	}
	InitManager::leaveThread(NULL);
	return 0;
}
