set(TargetName GraphicsAssets)

nau_collect_files(Sources
    DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/src
    MASK "*.cpp" "*.h"
    EXCLUDE
    "/platform/.*"
)

nau_collect_files(PublicHeaders
    DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
    MASK "*.h"
)

nau_add_module(${TargetName}
    SOURCES ${Sources} ${PublicHeaders}
    FOLDER "${NauEngineFolder}/core/modules"
    EXPORT_AS_LIBRARY
)

target_compile_definitions(${TargetName} PRIVATE NAU_GRAPHICSASSETS_BUILD)

target_precompile_headers(${TargetName} PRIVATE src/pch.h)
target_include_directories(${TargetName} PRIVATE
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(${TargetName} PRIVATE
    Render
    PlatformAppApi
    CoreScene
    CoreInput
    NauKernel
    D3DCompiler.lib
    imgui
    gainput
)

target_link_libraries(${TargetName} PUBLIC
    Render
    CoreInput
    CoreAssets
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX Sources FILES ${Sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX Headers FILES ${PublicHeaders})

target_include_directories(${TargetName} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/core/modules/graphics_assets/include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/core/modules/graphics_assets
    FILES_MATCHING 
      PATTERN "*.h"
      PATTERN "*.hpp"
      PATTERN "*.ipp"
)

nau_install(${TargetName} core)