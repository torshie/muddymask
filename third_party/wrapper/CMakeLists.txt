add_definitions(-DCMAKE_C_COMPILER=\"${CMAKE_C_COMPILER}\"
		-DCMAKE_CXX_COMPILER=\"${CMAKE_CXX_COMPILER}\"
		-DCMAKE_BINARY_DIR=\"${CMAKE_BINARY_DIR}\")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_executable(mycc compiler_wrapper.c)
add_executable(mycxx compiler_wrapper.c)
