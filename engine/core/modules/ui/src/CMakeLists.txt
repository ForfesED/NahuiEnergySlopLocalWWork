set(TargetName ui)

option(NAU_UI_CALLBACK_ON_ELEMNT_CHANGE "Allows to set callback function on element changed" OFF)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH ModuleRoot)

nau_collect_files(Sources
    DIRECTORIES ${ModuleRoot}/src/
    MASK "*.cpp" "*.h"
)

nau_collect_files(PublicHeaders
    DIRECTORIES ${ModuleRoot}/include/
    MASK "*.h"
)

nau_add_module(${TargetName}
    SOURCES ${Sources} ${PublicHeaders}
    FOLDER "${NauEngineFolder}/core/modules"
    EXPORT_AS_LIBRARY
    ENABLE_RTTI
)

nau_add_default_shaders(${TargetName} "${ModuleRoot}/src" shaders)

source_group(TREE ${ModuleRoot}/src/ PREFIX Sources FILES ${Sources})
source_group(TREE ${ModuleRoot}/include PREFIX Headers FILES ${PublicHeaders})

target_precompile_headers(${TargetName} PRIVATE pch.h)

target_include_directories(${TargetName} PUBLIC
  $<BUILD_INTERFACE:${ModuleRoot}/include>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/core/modules/ui/include>
)

if(BUILD_COCOS_WITH_DAGOR)
    target_compile_definitions(${TargetName} PRIVATE BUILD_COCOS_WITH_DAGOR=1)
    target_compile_definitions(cocos2d PRIVATE BUILD_COCOS_WITH_DAGOR=1)
endif()
target_compile_definitions(${TargetName} 
    PRIVATE NAU_COCOS2D_BUILD=1
    PRIVATE NAU_UI_BUILD
)


set_source_files_properties(${SourcesShaders} PROPERTIES
    HEADER_FILE_ONLY TRUE
)

target_link_libraries(${TargetName} PUBLIC
    cocos2d
)

target_link_libraries(${TargetName} PRIVATE
    GraphicsAssets
    Render
    CoreInput
    CoreScene
    Animation
    CoreAssets
    CoreScene
)

if (${NAU_UI_CALLBACK_ON_ELEMNT_CHANGE})
  target_compile_definitions(${TargetName} PUBLIC NAU_UI_CALLBACK_ON_ELEMNT_CHANGE=1)
endif()

target_link_libraries(${TargetName} PRIVATE ws2_32 userenv psapi winmm Version Iphlpapi)

install(DIRECTORY ${ModuleRoot}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/core/modules/ui
    FILES_MATCHING 
      PATTERN "*.h"
      PATTERN "*.hpp"
      PATTERN "*.ipp"
)

nau_install(${TargetName} core)
