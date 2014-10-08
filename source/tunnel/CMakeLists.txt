add_executable(muddy_tunnel main.cpp TunnelClient.cpp TunnelServer.cpp)
target_link_libraries(muddy_tunnel muddy_net muddy_tuntap
		muddy_crypto muddy_common)
if (WIN32)
	target_link_libraries(muddy_tunnel ws2_32 iphlpapi)
endif()
if ("${MUDDY_SYSTEM_NAME}" STREQUAL "android")
	target_link_libraries(muddy_tunnel gnustl_shared supc++)
endif()
