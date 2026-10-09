include(GoogleTest)

set(TargetName test_core_assets)

nau_collect_files(Sources
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  MASK "*.cpp" "*.h"
)

add_executable(${TargetName} ${Sources})
target_precompile_headers(${TargetName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
target_include_directories(${TargetName} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TargetName} PRIVATE
  TestCommonLib
  NauFramework
)

nau_add_compile_options(${TargetName})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})
set_target_properties (${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/tests"
)

nau_target_link_modules(${TargetName}
  CoreAssets
  CoreAssetFormats
)

gtest_discover_tests(${TargetName} DISCOVERY_TIMEOUT 30)
