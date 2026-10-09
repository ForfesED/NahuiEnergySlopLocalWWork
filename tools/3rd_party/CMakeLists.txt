# CMakeList.txt : CMake project for project_tool, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(json)
add_subdirectory(argparse)