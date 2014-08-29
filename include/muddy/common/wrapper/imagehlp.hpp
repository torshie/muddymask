#include <imagehlp.h>

namespace muddy { namespace wrapper {

inline void SymFromAddr_(HANDLE process, DWORD64 addr, PDWORD64 dis,
		PSYMBOL_INFO sym) {
	MUDDY_ASSURE_TRUE(SymFromAddr(process, addr, dis, sym));
}

inline void SymInitialize_(HANDLE h, PCTSTR path, BOOL i) {
	MUDDY_ASSURE_TRUE(SymInitialize(h, path, i));
}

}} // namespace muddy::wrapper
