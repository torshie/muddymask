if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
	set(CFG_MT_FLAGS /MTd)
	set(CFG_OPT_FLAGS /Od)
else()
	set(CFG_MT_FLAGS /MT)
	set(CFG_OPT_FLAGS /O2)
endif()

set(CFG_MISC_FLAGS /EHsc)
