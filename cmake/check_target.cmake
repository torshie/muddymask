include(CheckIncludeFiles)
include(CheckCXXSourceCompiles)

# Determines the socket API to be used.
# TODO The following list is incomplete
set(HDR_LIST sys/types.h sys/socket.h arpa/inet.h netinet/in.h)
check_include_files("${HDR_LIST}" HAVE_SOCK_UNIX)
unset(HDR_LIST)
if (NOT HAVE_SOCK_UNIX)
	check_include_files(winsock2.h HAVE_SOCK_WINSOCK2)
endif()

# Determines the type of system calls to be used.
# TODO The following list is incomplete
set(HDR_LIST sys/types.h sys/ioctl.h sys/stat.h fcntl.h unistd.h)
check_include_files("${HDR_LIST}" HAVE_SYSCALL_UNIX)
unset(HDR_LIST)
if (NOT HAVE_SYSCALL_UNIX)
	check_include_files(windows.h HAVE_SYSCALL_WIN32)
endif()

if (WIN32)
	set(HAVE_BACKWARD_PATH_SEPA ON)
else()
	set(HAVE_FORWARD_PATH_SEPA ON)
endif()

# Determines the TUN/TAP driver type.
check_include_files(linux/if_tun.h HAVE_TUNTAP_LINUX)
if (WIN32)
	set(HAVE_TUNTAP_WIN32 ON)
endif()

configure_file(${CMAKE_SOURCE_DIR}/cmake/config.h.in
		${CMAKE_BINARY_DIR}/config/muddy/config.h)
add_definitions(-DHAVE_MUDDY_CONFIG_H=1)
