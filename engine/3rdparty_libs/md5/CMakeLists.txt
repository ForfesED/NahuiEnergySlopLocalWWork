# CMakeList.txt : CMake project for lzma, include source and define
cmake_minimum_required(VERSION 3.20)

set(TargetName md5)

project(${TargetName} C)

set(HEADERS
	md5.h
)

set(SOURCES
	md5.c
)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

add_library(${TargetName} STATIC)

target_include_directories(${TargetName} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/3rdPartyLibs/md5>
)

target_sources(${TargetName} PRIVATE ${SOURCES} ${HEADERS}) 

#add_nau_compile_options(${TARGET_NAME})
#add_nau_folder_property(${TARGET_NAME} LIB)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})
