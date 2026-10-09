set(TargetName Animation)
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

target_precompile_headers(${TargetName} PRIVATE pch.h)

target_include_directories(${TargetName} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(${TargetName} PUBLIC
    $<BUILD_INTERFACE:${ModuleRoot}/include>  
    $<BUILD_INTERFACE:${ModuleRoot}/include/nau/generated>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/core/modules/animation/include>
#    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/core/modules/animation/include/nau/generated>
)

target_include_directories(${TargetName} PUBLIC
)

target_link_libraries(${TargetName} PUBLIC 
  CoreAssets
)

source_group(TREE ${ModuleRoot}/src PREFIX Sources FILES ${Sources})
source_group(TREE ${ModuleRoot}/include PREFIX Headers FILES ${PublicHeaders})

target_compile_definitions(${TargetName} PRIVATE NAU_ANIMATION_BUILD)

target_link_libraries(${TargetName} PUBLIC
  CoreAssets
  CoreScene
  ozz_base
  ozz_animation
)

target_link_libraries(${TargetName} PRIVATE
    Render
    imgui
)

install(DIRECTORY ${ModuleRoot}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/core/modules/animation
    FILES_MATCHING 
      PATTERN "*.h"
      PATTERN "*.hpp"
      PATTERN "*.ipp"
)

nau_install(${TargetName} core)