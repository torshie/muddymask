add_executable(client client.cpp)
target_link_libraries(client common adapter ws2_32)
