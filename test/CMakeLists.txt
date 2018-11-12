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
add_executable(test

	main.cpp
	${SOURCES}
)

## Link libraries
target_link_libraries(test
	
	sgl
	${GTEST_BOTH_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

## Include dirs
target_include_directories(test

	PUBLIC
		public
		${GTEST_INCLUDE_DIRS}
)

## Discover tests
gtest_discover_tests(test)