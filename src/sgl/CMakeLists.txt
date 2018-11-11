# Setup library ---------------------------------
## Gather source and headers files
file(GLOB_RECURSE SOURCES

	private/*.c
	private/*.cpp
	private/*.cxx
)
file(GLOB_RECURSE HEADERS public

	public/*.h
	public/*.hpp
	public/*.tpp
)

## Create library
add_library(${PROJECT_NAME}

	${SOURCES}
	${HEADERS}
)

## Include directories
target_include_directories(${PROJECT_NAME}

	PUBLIC
		public
)

# Setup test executable -------------------------
option(SGL_BUILD_TEST_EXECUTABLE "build test executable" ON)
if(SGL_BUILD_TEST_EXECUTABLE)
	# Add executable with sgl library
	add_executable(main main.cpp)

	# Set output directory
	set(TEST_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
	set_target_properties(main

		PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${TEST_RUNTIME_OUTPUT_DIRECTORY}
	)

	# Link sgl library
	target_link_libraries(main ${PROJECT_NAME})

	# Enable run target
	add_custom_target(run

		COMMAND				${TEST_RUNTIME_OUTPUT_DIRECTORY}/main
		DEPENDS				main
		WORKING_DIRECTORY	${PROJECT_SOURCE_DIR}
	)
endif(SGL_BUILD_TEST_EXECUTABLE)
