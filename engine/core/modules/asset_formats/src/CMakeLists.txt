
set(TargetName CoreAssetFormats)

nau_collect_files(Sources
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  
  MASK "*.cpp" "*.h"
)

nau_add_module(${TargetName}
  SOURCES ${Sources}
  FOLDER "${NauEngineFolder}/core/modules"
)

target_precompile_headers(${TargetName} PRIVATE pch.h)

target_include_directories(${TargetName} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TargetName} PRIVATE
  Animation
  CoreAssets
  CoreScene
  stb
  tinyimageformat
  ispc_texcomp
  tinydds
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Source FILES ${Sources})

nau_install(${TargetName} core)