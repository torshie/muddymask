include(CheckIncludeFiles)
include(CheckCXXSourceCompiles)

check_include_files(stdint.h HAVE_STDINT_H)
check_cxx_source_compiles(
		"#include <tr1/memory>
		using namespace std::tr1;
		int main() {}" HAVE_TR1_MEMORY)
check_cxx_source_compiles(
		"#include <netinet/in.h>
		in_addr_t x;
		int main() {}" HAVE_IN_ADDR_T)
check_cxx_source_compiles(
		"#include <winsock2.h>
		int main() {}" HAVE_WINSOCK2_H)

configure_file(${CMAKE_SOURCE_DIR}/config.hpp.cmake
		${CMAKE_BINARY_DIR}/cfg/config.hpp)
