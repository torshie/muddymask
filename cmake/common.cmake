# This .cmake file would provide several common functionalities(see bellow)
# if included. Generally this .cmake file should be included in the
# main project's top-level CMakeLists.txt
# Functionalities provided by this .cmake file:
# * A macro named as "muddy_join_string". It could be used to convert a
#   string list to a space separated string.
# * Variable MUDDY_BUILD_TYPE, could be "release" or "debug", meaning the
#   build type of the project.
# * Variable MUDDY_WERROR_FLAGS, empty string if option
#   MUDDY_DISABLE_WERROR is ON, "-Werror" otherwise.
# * Variable MUDDY_LTO_FLAGS, "-flto" in release build if option
#   MUDDY_ENABLE_LTO is ON and link-time-optimization is supported by the
#   compiler, in debug build or option MUDDY_ENALBE_LTO is off or
#   link-time-optimization isn't supported, it is an empty string.
# * Variable MUDDY_COMPILE_FLAGS, combination of MUDDY_OPT_FLAGS,
#   MUDDY_LTO_FLAGS and other harmless compiler flags. These flags are
#   supposed to be suitable for third party libraries.
# * Variable MUDDY_LINK_EXEC_FLAGS, various useful linker flags for
#   executables. These flags are supposed to be suitable for third party
#   libraries.
# * Variable MUDDY_LINK_SHARED_FLAGS, various useful linker flags for
#   shared libraries. These flags are supposed to be suitable for third
#   party libraries.
# * Set/Modify the following built-in variables:
#   - CMAKE_C_FLAGS
#   - CMAKE_C_FLAGS_DEBUG
#   - CMAKE_C_FLAGS_RELEASE
#   - CMAKE_CXX_FLAGS
#   - CMAKE_CXX_FLAGS_DEBUG
#   - CMAKE_CXX_FLAGS_RELEASE
#   - CMAKE_EXE_LINKER_FLAGS
#   - CMAKE_SHARED_LINKER_FLAGS
#   - CMAKE_INSTALL_RPATH
#   - CMAKE_BUILD_WITH_INSTALL_RPATH
#   - EXECUTALBE_OUTPUT_PATH
#   - LIBRARY_OUTPUT_PATH
#   Different from MUDDY_COMPILE_FLAGS and MUDDY_LINK_FLAGS, the variables
#   above are meant to be used by this project only, they may cause
#   difficulties for third party libraries.

# Macro muddy_join_string()
macro(muddy_join_string var)
	set(_var)
	foreach(_v ${${var}})
		set(_var "${_var} ${_v}")
	endforeach(_v ${${var}})
	string(STRIP ${_var} _var)
	set(${var} ${_var})
	unset(_var)
endmacro(muddy_join_string)

# Variable MUDDY_BUILD_TYPE
string(TOLOWER "${CMAKE_BUILD_TYPE}" MUDDY_BUILD_TYPE)
if (NOT "${MUDDY_BUILD_TYPE}" STREQUAL "release"
		AND NOT "${MUDDY_BUILD_TYPE}" STREQUAL "debug")
	set(ERROR_MESSAGE Please specify a build type with cmake command line
			argument \"-DCMAKE_BUILD_TYPE=debug\" or
			\"-DCMAKE_BUILD_TYPE=release\")
	muddy_join_string(ERROR_MESSAGE)
	message(FATAL_ERROR "${ERROR_MESSAGE}")
endif()

# Command line macro definition: MUDDY_LOGGING_LEVEL
set(HELP_MESSAGE "Logging level, could be one of: kTrace, kDebug, "
		"kVerbose, kInfo, kNotice, kWarning, kError, kFatal")
muddy_join_string(HELP_MESSAGE)
set(MUDDY_LOGGING_LEVEL "" CACHE STRING "${HELP_MESSAGE}")
unset(HELP_MESSAGE)
if (NOT "${MUDDY_LOGGING_LEVEL}" STREQUAL "")
	add_definitions("-DMUDDY_LOGGING_LEVEL=${MUDDY_LOGGING_LEVEL}")
endif()
unset(MUDDY_LOGGING_LEVEL)

# Command line macro definition: MUDDY_ENABLE_DEBUGGING
if ("${MUDDY_BUILD_TYPE}" STREQUAL "debug")
	add_definitions(-DMUDDY_ENABLE_DEBUGGING=1)
endif()

# Option MUDDY_ENABLE_LTO
include(CheckCXXCompilerFlag)
set(HELP_MESSAGE "Enable link-time-optimization, enabled if supported "
		"by the compiler and CMAKE_BUILD_TYPE=release. Strongly "
		"recommended for C++ projects.")
muddy_join_string(HELP_MESSAGE)
option(MUDDY_ENABLE_LTO "${HELP_MESSAGE}" ON)
unset(HELP_MESSAGE)

# Option MUDDY_DISABLE_WERROR
set(HELP_MESSAGE "Do not treat compiler warnings as errors [default=OFF]")
option(MUDDY_DISABLE_WERROR "${HELP_MESSAGE}" OFF)
unset(HELP_MESSAGE)

# RPATH and output directory related variables
set(CMAKE_INSTALL_RPATH
		$ORIGIN
		$ORIGIN/../lib
		$ORIGIN/../third_party/lib)
string(REPLACE ";" ":" CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

if (MSVC)
	include(cmake/msvc.cmake)
else()
	include(cmake/gcc.cmake)
endif()

unset(MUDDY_ENABLE_LTO)
unset(MUDDY_DISABLE_WERROR)
