# CMakeList.txt : CMake project for lzma, include source and define
cmake_minimum_required(VERSION 3.20)

set(TargetName lzma-9.20)

project(${TargetName} C)

set(HEADERS
	LzFind.h
	LzHash.h
	LzmaDec.h
	LzmaEnc.h
	LzmaLib.h
	Types.h
)

set(SOURCES
	LzFind.c
	LzmaDec.c
	LzmaEnc.c
)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

add_library(${TargetName} STATIC)
add_library(3rdPartyLibs_lzma ALIAS ${TargetName})

target_sources(${TargetName} PRIVATE ${HEADERS} ${SOURCES})
target_compile_definitions(${TargetName} PRIVATE _7ZIP_ST __B_MINICORE)

target_include_directories(${TargetName} INTERFACE  
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include/3rdPartyLibs/lzma-9.20> 
)

#add_nau_compile_options(lzma-9.20)
#add_nau_folder_property(lzma-9.20 LIB arc)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})
