#ifndef MUDDY_COMMON_INIT_MANAGER_HPP_INCLUDED_
#define MUDDY_COMMON_INIT_MANAGER_HPP_INCLUDED_

#include <cstdlib>
#include <muddy/common/except.hpp>

namespace muddy {

class InitManager {
	friend class Thread;
public:
	enum Priority {
		kSliceAllocator = 5000, kWinsock = 6000
	};

	InitManager() {
		initMain();
	}

	~InitManager() {
		leaveMain();
	}

	static void setup(void (*ctor)(), void (*dtor)(), int priority,
			bool thread, const char* description = NULL) {
		if (mainInitInvoked) {
			throw Unpossible("invoking InitManager::setup() after "
					"InitManager::initialize() isn't allowed");
		}
		Initializer i = {
			ctor, dtor, priority, thread,
			(description == NULL) ? "(unknown)" : description
		};
		initTable[initializerCount++] = i;
	}

private:
	static void initMain() {
		std::qsort(initTable, initializerCount, sizeof(Initializer),
				&Initializer::cmp);
		for (int i = 0; i < initializerCount; ++i) {
			if (initTable[i].ctor == NULL) {
				continue;
			}
			initTable[i].ctor();
		}
		mainInitInvoked = true;
	}

	static void leaveMain() {
		for (int i = initializerCount - 1; i >= 0; --i) {
			if (initTable[i].dtor == NULL) {
				continue;
			}
			initTable[i].dtor();
		}
	}

	static void initThread() {
		if (!mainInitInvoked) {
			throw Unpossible("trying to initialize thread before main()"
					" or initMain() hasn't been invoked.");
		}
		for (int i = 0; i < initializerCount; ++i) {
			if (!initTable[i].thread || initTable[i].ctor == NULL) {
				continue;
			}
			initTable[i].ctor();
		}
	}

	static void leaveThread(void* /* argument */ ) {
		for (int i = initializerCount - 1; i >= 0; --i) {
			if (!initTable[i].thread || initTable[i].dtor == NULL) {
				continue;
			}
			initTable[i].dtor();
		}
	}

private:
	enum { kTableSize = 100 };

	struct Initializer {
		void (*ctor)();
		void (*dtor)();
		int priority;
		bool thread;
		const char* description;

		static int cmp(const void* a, const void* b) {
			Initializer* a1 = (Initializer*)a;
			Initializer* b1 = (Initializer*)b;
			if (a1->priority == b1->priority) {
				throw Unpossible("duplicated priorities aren't allowed");
			}
			return a1->priority - b1->priority;
		}
	};

	static int initializerCount;
	static bool mainInitInvoked;
	static Initializer initTable[kTableSize];
};

} // namespace muddy

#endif // MUDDY_COMMON_INIT_MANAGER_HPP_INCLUDED_
