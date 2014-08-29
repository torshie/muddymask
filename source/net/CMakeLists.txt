if (WIN32)
	set(MUDDY_PLATFORM_SOURCE
			DatagramSocket_win32.cpp
			StartWSA.cpp)
else()
	set(MUDDY_PLATFORM_SOURCE
			DatagramSocket_unix.cpp)
endif()

add_library(muddy_net STATIC
		DatagramSocket.cpp
		${MUDDY_PLATFORM_SOURCE}
		)
unset(MUDDY_PLATFORM_SOURCE)
