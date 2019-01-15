cmake_minimum_required(VERSION 3.12)

# Project setup ---------------------------------
project(sgl

	C
	CXX
)

## Platform setup
include(CMakePlatform.cmake)

## Versioning
set(SGL_VERSION_MAJOR 0)
set(SGL_VERSION_MINOR 0)
set(SGL_VERSION_PATCH 0) # Not really using it

## Build type config
set(CMAKE_CXX_FLAGS_DEBUG			"-DSGL_BUILD_DEBUG=1 -ggdb -O0")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"-DSGL_BUILD_DEVELOPMENT=1 -ggdb -O3")
set(CMAKE_CXX_FLAGS_RELEASE			"-DSGL_BUILD_RELEASE=1 -Ofast")

# Compiler setup --------------------------------
## CXX preferences
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAG} -mavx2 -fopenmp -pthread") # Not sure about OpenMP

# Code setup ------------------------------------
## Third party
add_subdirectory(vendor)

## Source code
add_subdirectory(src)

## Test setup -----------------------------------
option(SGL_UNIT_TEST "enable unit testing" ON)
if(SGL_UNIT_TEST)
	add_subdirectory(test)
endif(SGL_UNIT_TEST)