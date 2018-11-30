# Setup unit testing ----------------------------
## Include Google Test and enable testing
include(GoogleTest)
enable_testing()

## Find required packages
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

## Compile tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
file(GLOB_RECURSE SOURCES private

	*.c
	*.cpp
	*.cxx
)
add_executable(${PROJECT_NAME}_test

	main.cpp
	${SOURCES}
)

## Link libraries
target_link_libraries(${PROJECT_NAME}_test
	
	sgl
	${GTEST_BOTH_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

## Include dirs
target_include_directories(${PROJECT_NAME}_test

	PUBLIC
		public
		${GTEST_INCLUDE_DIRS}
)

## Discover tests
gtest_discover_tests(${PROJECT_NAME}_test

	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

## Setup post-build command to run tests
add_custom_target(test

	COMMAND				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_test
	DEPENDS				${PROJECT_NAME}_test
	WORKING_DIRECTORY	${PROJECT_SOURCE_DIR}
	COMMENT				"Run all unit tests"
)