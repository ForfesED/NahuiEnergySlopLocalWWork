#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)

set(TargetName EASTL)

project(${TargetName} CXX)

#-------------------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------------------
option(EASTL_BUILD_BENCHMARK "Enable generation of build files for benchmark" OFF)
option(EASTL_BUILD_TESTS "Enable generation of build files for tests" OFF)

#-------------------------------------------------------------------------------------------
# Compiler Flags
#-------------------------------------------------------------------------------------------
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/scripts/CMake")
include(CommonCppFlags)

#-------------------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------------------
file(GLOB EASTL_SOURCES "source/*.cpp")
add_library(${TargetName} ${EASTL_SOURCES})

if(EASTL_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

if(EASTL_BUILD_TESTS)
    add_subdirectory(test)
endif()

#-------------------------------------------------------------------------------------------
# Defines
#-------------------------------------------------------------------------------------------
add_definitions(-D_CHAR16T)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-DEASTL_OPENSOURCE=1)

#-------------------------------------------------------------------------------------------
# Include dirs
#-------------------------------------------------------------------------------------------
target_include_directories(${TargetName} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/EASTL/include>
)


if(BUILD_SHARED_LIBS)

  target_compile_definitions(${TargetName} PUBLIC
    EA_DLL
    EASTL_DLL=1
  )
  target_compile_definitions(${TargetName} PUBLIC EA_DLL)
  target_compile_definitions(${TargetName} PRIVATE EASTL_BUILD)

else()
  target_compile_definitions(${TargetName} PUBLIC
    EASTL_DLL=0
  )
endif()

#-------------------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------------------
if (NOT TARGET EABase)
    add_subdirectory(test/packages/EABase)
endif()

target_link_libraries(${TargetName} EABase)

# Install
nau_collect_files(HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  MASK "*.h" "*.hpp" "*.ipp"
)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})