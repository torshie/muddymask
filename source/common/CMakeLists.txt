if (WIN32)
	set(MUDDY_PLATFORM_SOURCE
			Registry.cpp
			StackTrace_win32.cpp
			mm/AlignedAllocator_win32.cpp
			)
else()
	set(MUDDY_PLATFORM_SOURCE
			mm/AlignedAllocator_unix.cpp
			)
	if ("${MUDDY_SYSTEM_NAME}" STREQUAL "android")
		set(MUDDY_PLATFORM_SOURCE
				StackTrace_android.cpp
				${MUDDY_PLATFORM_SOURCE})
	else()
		set(MUDDY_PLATFORM_SOURCE
				StackTrace_unix.cpp
				${MUDDY_PLATFORM_SOURCE})
	endif()
endif()

add_library(muddy_common STATIC EXCLUDE_FROM_ALL
		except.cpp
		logging/Logger.cpp
		thread/shared_mutex.cpp
		util/net.cpp
		util/path.cpp
		util/misc.cpp
		${MUDDY_PLATFORM_SOURCE}
		)
unset(MUDDY_PLATFORM_SOURCE)

if (UNIX AND NOT "${MUDDY_SYSTEM_NAME}" STREQUAL "android")
	add_dependencies(muddy_common build_backtrace)
	target_link_libraries(muddy_common backtrace)
endif()
