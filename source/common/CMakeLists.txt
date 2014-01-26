if (WIN32)
	set(MUDDY_PLATFORM_SOURCE
			Registry.cpp
			StackTrace_win32.cpp
			mm/AlignedAllocator_win32.cpp
			)
else()
	if (NOT APPLE)
		set(MUDDY_PLATFORM_SOURCE
				StackTrace_unix.cpp
				mm/AlignedAllocator_unix.cpp
				)
	else()
		# TODO complete this branch
	endif()
endif()

add_library(muddy_common STATIC EXCLUDE_FROM_ALL
		except.cpp
		util/path.cpp
		${MUDDY_PLATFORM_SOURCE}
		)
unset(MUDDY_PLATFORM_SOURCE)

if (UNIX)
	add_dependencies(muddy_common build_backtrace)
	target_link_libraries(muddy_common backtrace)
endif()
