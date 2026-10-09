cmake_minimum_required(VERSION 3.16)

project(stb LANGUAGES CXX)

set(TargetName stb)

add_library(${TargetName} 
  INTERFACE
)

target_include_directories(${TargetName} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/stb>
)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})