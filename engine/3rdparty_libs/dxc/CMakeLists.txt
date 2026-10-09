cmake_minimum_required(VERSION 3.16)

project(dxc
  VERSION 1.0
  LANGUAGES CXX
  )

set(TargetName dxc)

nau_collect_files(Sources
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  MASK "*.h" "*.hpp"
)

add_library(${TargetName} INTERFACE ${Sources})

target_include_directories(${TargetName} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/dxc/include>
)

nau_collect_files(HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  MASK "*.h" "*.hpp" "*.ipp"
)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})
