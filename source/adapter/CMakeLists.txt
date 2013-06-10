if (${WIN32})
add_library(adapter STATIC
		Tuntap_win32.cpp
		)
endif()
