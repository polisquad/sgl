cmake_minimum_required(VERSION 3.12)

# Project setup ---------------------------------
project(sgl)

# Compiler setup --------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Code setup ------------------------------------
### Vendors ###
#add_subdirectory(vendor)

### Source code ###
add_subdirectory(src)