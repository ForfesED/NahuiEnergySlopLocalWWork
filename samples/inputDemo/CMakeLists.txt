nau_collect_files(SOURCES
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  MASK "*.cpp" "*.h"
  EXCLUDE
    "/platform/.*"
)

add_executable(inputDemo 
  ${SOURCES}
)

target_link_libraries(inputDemo PRIVATE
  SampleCommonLib 
  CoreInput
  imgui
  Graphics
)

target_precompile_headers(inputDemo PRIVATE pch.h)

nau_target_link_modules(inputDemo
  PlatformApp
  CoreInput
  Graphics
  CoreScene
  CoreAssets
  GraphicsAssets
  DebugRenderer
  CoreAssetFormats
)

nau_add_compile_options(inputDemo)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

set_target_properties (inputDemo PROPERTIES
    FOLDER "${NauEngineFolder}/samples"
)