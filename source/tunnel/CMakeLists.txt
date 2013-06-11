if (${WIN32})
	set(SOCKLIB ws2_32)
endif()

add_executable(client client.cpp)
target_link_libraries(client common adapter ${SOCKLIB})

add_executable(server server.cpp)
target_link_libraries(server common adapter ${SOCKLIB})
