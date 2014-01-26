set(BACKTRACE libbacktrace)
add_custom_target(build_backtrace
		COMMAND [ -f build/build_backtrace ]
			|| ( rm -rf build/${BACKTRACE}
			&& mkdir build/${BACKTRACE}
			&& cd build/${BACKTRACE}
			&& ${MUDDY_SETENV}
				${CMAKE_CURRENT_SOURCE_DIR}/${BACKTRACE}/backtrace/configure
				"CFLAGS=${MUDDY_COMPILE_FLAGS} -pthread"
				"CXXFLAGS=${MUDDY_COMPILE_FLAGS} -pthread"
			&& make -j ${MUDDY_JOB_NUMBER}
			&& cd -
			&& install -v -m 0644 -T -D
				`find build/${BACKTRACE} -name libbacktrace.a`
				lib/libbacktrace.a
			&& install -v -m 0644 -T -D
				${CMAKE_CURRENT_SOURCE_DIR}/${BACKTRACE}/backtrace/backtrace.h
				include/backtrace/backtrace.h )
		COMMAND touch build/build_backtrace
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/third_party)
