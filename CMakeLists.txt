cmake_minimum_required(VERSION 3.12)

# Project setup ---------------------------------
project(sgl

	C
	CXX
)

# Compiler setup --------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -ggdb -O1")

# Code setup ------------------------------------
## Vendors
#add_subdirectory(vendor)

## Source code
add_subdirectory(src)

## Test setup -----------------------------------
option(SGL_UNIT_TEST "enable unit testing" ON)
if(SGL_UNIT_TEST)
	add_subdirectory(test)
endif(SGL_UNIT_TEST)