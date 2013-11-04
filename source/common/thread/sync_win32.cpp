#include <muddy/common/thread/sync.hpp>

using namespace muddy;

void ReadWriteLock::lockRead() {
	Guard a(writeMutex);
	Guard b(counterMutex);
	++readerCount;
	if (readerCount == 1) {
		ResetEvent(noReader);
	}
}

void ReadWriteLock::unlockRead() {
	Guard g(counterMutex);
	--readerCount;
	if (readerCount == 0) {
		SetEvent(noReader);
	}
}
