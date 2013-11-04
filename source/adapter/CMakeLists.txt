if (WIN32)
	set(TUNTAP_SOURCE Tuntap_win32.cpp)
else()
	set(TUNTAP_SOURCE Tuntap_linux.cpp)
endif()

add_library(adapter STATIC
		${TUNTAP_SOURCE}
		)
