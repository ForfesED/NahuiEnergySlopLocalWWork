cmake_minimum_required(VERSION 3.20)

set(TargetName asio)

project(${TargetName} LANGUAGES CXX)

add_library(${TargetName} 
  INTERFACE
)

target_include_directories(${TargetName} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asio/include>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/asio/asio/include>
)

nau_collect_files(HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/asio/include
  MASK "*.h" "*.hpp" "*.ipp"
)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})