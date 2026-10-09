set(TargetName PhysicsSample)

nau_collect_files(Sources
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  MASK "*.cpp" "*.h"
)

add_executable(${TargetName}
  ${Sources}
)
 
target_link_libraries(${TargetName} PRIVATE
  SampleCommonLib
  imgui
  Physics
)
target_precompile_headers(${TargetName} PRIVATE pch.h)

nau_target_link_modules(${TargetName}
  PlatformApp
  CoreScene
  CoreAssets
  GraphicsAssets
  DebugRenderer
  CoreAssetFormats
  CoreInput
  Graphics
  Physics
  PhysicsJolt
)

nau_add_compile_options(${TargetName})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})
set_target_properties (${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/samples"
)
