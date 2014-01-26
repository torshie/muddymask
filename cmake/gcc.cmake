# Variable MUDDY_LTO_FLAGS
check_cxx_compiler_flag("-flto" MUDDY_CXX_HAS_LTO)
if ("${MUDDY_BUILD_TYPE}" STREQUAL "release" AND MUDDY_CXX_HAS_LTO
		AND MUDDY_ENABLE_LTO)
	set(MUDDY_LTO_FLAGS -flto)
else()
	set(MUDDY_LTO_FLAGS "")
endif()
unset(MUDDY_CXX_HAS_LTO)

# Variable MUDDY_OPT_FLAGS
if ("${MUDDY_BUILD_TYPE}" STREQUAL debug)
	set(MUDDY_OPT_FLAGS -O0)
else()
	set(MUDDY_OPT_FLAGS -O3)
endif()

# Variable MUDDY_COMPILE_FLAGS
set(MUDDY_COMPILE_FLAGS "-g -pipe ${MUDDY_OPT_FLAGS} ${MUDDY_LTO_FLAGS}")

# Variable MUDDY_LINK_EXEC_FLAGS
set(MUDDY_LINK_EXEC_FLAGS "${MUDDY_LTO_FLAGS}")

# Variable MUDDY_LINK_SHARED_FLAGS
set(MUDDY_LINK_SHARED_FLAGS "${MUDDY_LTO_FLAGS}")

# Variable CMAKE_C_FLAGS
set(CMAKE_C_FLAGS ${MUDDY_COMPILE_FLAGS} ${MUDDY_WERROR_FLAGS} -Wall
		-Wextra -std=c99)
muddy_join_string(CMAKE_C_FLAGS)
set(CMAKE_C_FLAGS_DEBUG "-fno-omit-frame-pointer -fno-inline")
set(CMAKE_C_FLAGS_RELEASE "")

# Variable CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS ${MUDDY_COMPILE_FLAGS} ${MUDDY_WERROR_FLAGS} -Wall
		-Wextra -std=c++11)
muddy_join_string(CMAKE_CXX_FLAGS)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

# Varialbe CMAKE_EXEC_LINKER_FLAGS & CMAKE_SHARED_LINKER_FLAGS
set(CMAKE_EXE_LINKER_FLAGS ${MUDDY_LINK_EXEC_FLAGS})
muddy_join_string(CMAKE_EXE_LINKER_FLAGS)
set(CMAKE_SHARED_LINKER_FLAGS ${MUDDY_LINK_SHARED_FLAGS})
muddy_join_string(CMAKE_SHARED_LINKER_FLAGS)

if (UNIX)
	macro(muddy_append_pthread var)
		set($var "${${var}} -pthread")
	endmacro(muddy_append_pthread)
	muddy_append_pthread(CMAKE_C_FLAGS)
	muddy_append_pthread(CMAKE_CXX_FLAGS)
	muddy_append_pthread(CMAKE_EXE_LINKER_FLAGS)
	muddy_append_pthread(CMAKE_SHARED_LINKER_FLAGS)
endif()
