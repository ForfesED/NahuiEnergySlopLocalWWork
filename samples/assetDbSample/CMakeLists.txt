set(TargetName AssetDbSample)

nau_collect_files(Sources
    DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    MASK "*.cpp" "*.h"
)

add_executable(${TargetName} ${Sources})

target_link_libraries(${TargetName} PRIVATE
    SampleCommonLib
    Animation
    Physics
    Graphics
)

target_precompile_headers(${TargetName} PRIVATE pch.h)

nau_collect_files(SourcesMaterials
    DIRECTORIES content/materials
    MASK "*.json"
    EXCLUDE
    "/platform/.*"
)
source_group(shaders/materials FILES ${SourcesMaterials})
target_sources(${TargetName} PRIVATE ${SourcesMaterials})

nau_target_link_modules(${TargetName}
    PlatformApp
    Animation
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

set_target_properties(${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/samples"
)