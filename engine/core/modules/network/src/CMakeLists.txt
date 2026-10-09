set(TargetName Network)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH ModuleRoot)

nau_collect_files(Sources
  DIRECTORIES ${ModuleRoot}/src
  RELATIVE ${ModuleRoot}/src
  
  MASK "*.cpp" "*.h"
  EXCLUDE
    "/platform/.*"
)

nau_collect_files(PublicHeaders
  DIRECTORIES ${ModuleRoot}/include
  RELATIVE ${ModuleRoot}
  
  MASK "*.h"
  EXCLUDE
    "/platform/.*"
  PREPEND "../"
)


nau_add_module(${TargetName}
  SOURCES ${Sources} ${PublicHeaders}
  FOLDER "${NauEngineFolder}/core/modules"
  EXPORT_AS_LIBRARY
)

target_compile_definitions(${TargetName} PRIVATE NAU_NETWORK_BUILD)

target_precompile_headers(${TargetName} PRIVATE pch.h)

target_include_directories(${TargetName} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(${TargetName} PUBLIC
  $<BUILD_INTERFACE:${ModuleRoot}/include>
  $<BUILD_INTERFACE:${ModuleRoot}/include/nau/generated>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/core/modules/network/include>
)

target_link_libraries(${TargetName} PUBLIC 
  asio
#  GameNetworkingSockets 
  uriparser
  CoreScene
)

source_group(TREE ${ModuleRoot}/src PREFIX Sources FILES ${Sources})
source_group(TREE ${ModuleRoot}/include PREFIX Headers FILES ${PublicHeaders})

install(DIRECTORY ${ModuleRoot}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/core/modules/network
    FILES_MATCHING 
      PATTERN "*.h"
      PATTERN "*.hpp"
      PATTERN "*.ipp"
)

nau_install(${TargetName} core)