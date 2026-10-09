set(TargetName UiSampleApp)

set(Sources
    main.cpp
    samples/scroll_sample.h
    samples/scroll_sample.cpp
)
add_executable(${TargetName} ${Sources})

target_link_libraries(${TargetName} PRIVATE
    SampleCommonLib
    ui
    Animation
)

target_link_libraries(${TargetName} PRIVATE ws2_32 userenv psapi winmm Version Iphlpapi)

target_precompile_headers(${TargetName} PRIVATE pch.h)

nau_target_link_modules(${TargetName}
    PlatformApp
    Graphics
    DebugRenderer
    CoreScene
    CoreInput
    CoreAssets
    CoreAssetFormats
    GraphicsAssets
    ui
    Animation
)

if(NOT NauSDKLibraries)
 add_dependencies(${TargetName} SceneBaseSample)
endif()

nau_add_compile_options(${TargetName})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})
set_target_properties (${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/samples"
)

target_include_directories(${TargetName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT NauSDKLibraries)
  if (NOT COCOS2DX_ROOT_PATH)
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../engine/core/modules/ui/cocos2d-x)
  endif()

  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../sceneBase/content/shaders DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../Resources)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../sceneBase/resources/materials DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../Resources)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../../engine/core/modules/ui/src/shaders DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/ui)

  set(ResDir ${CMAKE_CURRENT_SOURCE_DIR}/../Resources)

  nau_add_default_shaders(${TargetName} ${ResDir} shaders
    INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../engine/core/modules/render/include/nau/shaders
  )
endif()