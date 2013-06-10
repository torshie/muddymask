set(CFG_MT_FLAGS -pthread)

check_cxx_compiler_flag(-flto CXX_HAS_LTO)
if ("${CXX_HAS_LTO}" AND "${CFG_BUILD_TYPE}" STREQUAL "release")
	set(CFG_LTO_FLAGS -flto)
endif()
unset(CXX_HAS_LTO)

if ("${CFG_BUILD_TYPE}" STREQUAL "debug")
	set(CFG_OPT_FLAGS "-O0")
else()
	set(CFG_OPT_FLAGS "-O3")
endif()

set(CFG_MISC_FLAGS -Wextra -Wall)
if (NOT "${DISABLE_WERROR}")
	unset(CFG_MISC_FLAGS)
endif()
unset(DISABLE_WERROR)
