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

## Manage dependencies
### OpenGL
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

if(NOT SDL2_FOUND)
	message(FATAL_ERROR "SDL2 not found! `sudo apt-get install libsdl2-dev`")
endif(NOT SDL2_FOUND)

if(NOT OpenGL_FOUND)
	message(FATAL_ERROR "No GL library found!")
endif(NOT OpenGL_FOUND)

## Link third-party libraries
target_link_libraries(${PROJECT_NAME}

	${SDL2_LIBRARIES}
	${OPENGL_LIBRARIES}
)

## Include directories
target_include_directories(${PROJECT_NAME}

	PUBLIC
		public
		${SDL2_INCLUDE_DIRS}
		${OPENGL_INCLUDE_DIR}
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
