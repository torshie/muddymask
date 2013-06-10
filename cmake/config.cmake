include(CheckIncludeFiles)
include(CheckCXXSourceCompiles)

check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(tr1/memory HAVE_TR1_MEMORY)
check_cxx_source_compiles(
	"#include <netinet/in.h>
	in_addr_t x;
	int main() {}" HAVE_IN_ADDR_T)

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake
		${CMAKE_BINARY_DIR}/config.h)
