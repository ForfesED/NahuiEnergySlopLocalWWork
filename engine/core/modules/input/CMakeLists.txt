set(TargetName CoreInput)

set(ModuleRoot ${CMAKE_CURRENT_SOURCE_DIR})

nau_collect_files(Sources
  RELATIVE ${ModuleRoot}
  DIRECTORIES
    ${ModuleRoot}/src
    ${ModuleRoot}/include
  MASK "*.cpp" "*.h"
  EXCLUDE
    "/platform/.*"
)

if (${Platform_Windows})

  nau_collect_files(Sources
    DIRECTORIES 
      ${ModuleRoot}/src/platform/windows
      ${ModuleRoot}/include/platform/windows
    RELATIVE ${ModuleRoot}
    MASK "*.cpp" "*.h"
  )
endif()

nau_add_module(${TargetName}
  SOURCES ${Sources}
  FOLDER "${NauEngineFolder}/core/modules"
  EXPORT_AS_LIBRARY
)

target_link_libraries(${TargetName} PRIVATE
  gainput PlatformAppApi)

target_include_directories(${TargetName} PRIVATE 
  ${ModuleRoot}/src)

target_include_directories(${TargetName} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/core/modules/input/include>
)

target_precompile_headers(${TargetName} PRIVATE src/pch.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})

## Module API
nau_collect_files(ApiHeaders
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
  MASK "*.h"
)

target_compile_definitions(${TargetName} PRIVATE
  NAU_COREINPUT_BUILD
)

install(DIRECTORY ${ModuleRoot}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/core/modules/input
    FILES_MATCHING 
      PATTERN "*.h"
      PATTERN "*.hpp"
      PATTERN "*.ipp"
)

nau_install(${TargetName} core)