project(MUDDY)
cmake_minimum_required(VERSION 2.6)

include(cmake/common.cmake)

include_directories(include ${CMAKE_BINARY_DIR}/third_party/include)

add_subdirectory(source)
add_subdirectory(third_party)
