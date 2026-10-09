
set(TargetName test_platform_app)


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
  gtest
  gmock
  NauKernel
  PlatformApp
  CoreAssets
)

nau_target_link_modules(${TargetName}
  PlatformApp
#  ScriptsLua
  CoreAssets
)


#if(MSVC)
#  set_property(TARGET PlatformApp PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS true)
#  target_link_libraries(${TargetName} PRIVATE delayimp)
#  target_link_options(${TargetName} PRIVATE "/DELAYLOAD:$<TARGET_FILE_BASE_NAME:PlatformApp>.dll")
#endif()


nau_add_compile_options(${TargetName})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})
set_target_properties (${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/tests"
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${TargetName} PRIVATE NAU_STATIC_RUNTIME)
endif()

include(GoogleTest)
gtest_discover_tests(${TargetName} DISCOVERY_TIMEOUT 10)