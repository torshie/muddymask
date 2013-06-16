set(GENERIC_SOURCE
		except.cpp
		ConfigParser.cpp
		Configuration.cpp
		InitManager.cpp
		inet/DatagramSocket.cpp
		crypto/hash.cpp
		crypto/Dummy.cpp
		util/inet.cpp
		util/misc.cpp
		)

set(WIN32_SOURCE
		Registry.cpp
		thread/Thread_win32.cpp
		)

set(POSIX_SOURCE
		thread/Thread_posix.cpp
		)

if (${WIN32})
	add_library(common STATIC ${GENERIC_SOURCE} ${WIN32_SOURCE})
else()
	add_library(common STATIC ${GENERIC_SOURCE} ${POSIX_SOURCE})
endif()
