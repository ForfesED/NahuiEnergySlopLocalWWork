# CMakeList.txt : CMake project for fast_float, headeronly
# git:
cmake_minimum_required(VERSION 3.20)

set(TargetName fast_float)

project(${TargetName})

add_library(${TargetName} INTERFACE)

set(HEADERS
	ascii_number.h
	bigint.h
	decimal_to_binary.h
	digit_comparison.h
	fast_float.h
	fast_table.h
	float_common.h
	parse_number.h
	simple_decimal_conversion.h
)

source_group("Headers" FILES ${HEADERS})

target_sources(${TargetName} PRIVATE ${HEADERS})

target_include_directories(${TargetName} INTERFACE  
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include/3rdPartyLibs/fast_float> 
)

#add_nau_folder_property(fast_float LIB)

nau_install(${TargetName} "3rdPartyLibs" ${HEADERS})
