set(GENERIC_SOURCE
		except.cpp
		ConfigParser.cpp
		Configuration.cpp
		InitManager.cpp
		crypto/hash.cpp
		crypto/Dummy.cpp
		util/inet.cpp
		util/misc.cpp
		)

set(WIN32_SOURCE
		Registry.cpp
		inet/DatagramSocket_win32.cpp
		thread/Thread_win32.cpp
		)

if (${WIN32})
	add_library(common STATIC ${GENERIC_SOURCE} ${WIN32_SOURCE})
endif()
