#ifndef MUDDY_COMMON_THREAD_WIN32_HPP_INCLUDED_
#define MUDDY_COMMON_THREAD_WIN32_HPP_INCLUDED_

namespace muddy {

class Thread {
public:
	explicit Thread(void (*entry)(void*), void* arg = NULL);
	~Thread();

	void wait();

private:
	uintptr_t tid;

	static unsigned __stdcall call(void* p);
};

} // namespace muddy

#endif // MUDDY_COMMON_THREAD_WIN32_HPP_INCLUDED_
