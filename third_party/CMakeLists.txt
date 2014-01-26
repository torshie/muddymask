add_custom_target(setup_3rd_party_env
		COMMAND ${CMAKE_COMMAND} -E make_directory third_party
		COMMAND ${CMAKE_COMMAND} -E make_directory third_party/build
		COMMAND ${CMAKE_COMMAND} -E make_directory third_party/lib
		COMMAND ${CMAKE_COMMAND} -E make_directory third_party/include
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

if (CMAKE_HOST_UNIX)
	execute_process(
			COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/job_number.sh
			OUTPUT_VARIABLE MUDDY_JOB_NUMBER
			OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

if (UNIX)
	add_subdirectory(wrapper)
	set(MUDDY_SETENV CC=${CMAKE_CURRENT_BINARY_DIR}/wrapper/mycc
			CXX=${CMAKE_CURRENT_BINARY_DIR}/wrapper/mycxx)
	add_dependencies(setup_3rd_party_env mycc mycxx)
endif()

if (UNIX)
	include(backtrace_unix.cmake)
	add_dependencies(build_backtrace setup_3rd_party_env)
endif()
