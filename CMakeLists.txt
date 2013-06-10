project(cmake)
cmake_minimum_required(VERSION 2.6)

include(CheckCXXCompilerFlag)
include(cmake/config.cmake)
if (${MSVC})
	include(cmake/vc.cmake)
else()
	include(cmake/gcc.cmake)
endif()

macro(join_string var)
	set(_var)
	foreach(_v ${${var}})
		set(_var "${_var} ${_v}")
	endforeach(_v ${${var}})
	string(STRIP ${_var} _var)
	set(${var} ${_var})
	unset(_var)
endmacro(join_string)

string(TOLOWER "${CMAKE_BUILD_TYPE}" CFG_BUILD_TYPE)
if (NOT "${CFG_BUILD_TYPE}" STREQUAL "release"
		AND NOT "${CFG_BUILD_TYPE}" STREQUAL "debug")
	set(ERROR_MESSAGE Please specify a build type with cmake command line
			argument \"-DCMAKE_BUILD_TYPE=debug\" or
			\"-DCMAKE_BUILD_TYPE=release\")
	join_string(ERROR_MESSAGE)
	message(FATAL_ERROR "${ERROR_MESSAGE}")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
	add_definitions(-DENABLE_DEBUGGING_CODE=1)
endif()

include_directories(include ${CMAKE_BINARY_DIR})

set(LINKER_FLAGS "${CFG_MT_FLAGS} ${CFG_LTO_FLAGS}")
set(CMAKE_CXX_FLAGS ${CFG_MISC_FLAGS} ${CFG_OPT_FLAGS} ${CFG_LTO_FLAGS})
join_string(CMAKE_CXX_FLAGS)
set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})
set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(source)
