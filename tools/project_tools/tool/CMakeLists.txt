set(TargetName "ProjectTool")

nau_collect_files(Sources
  ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  MASK "*.cpp"
)

nau_collect_files(Public
  ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  MASK "*.h"
)

set(Files ${Sources} ${Public})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Files})

if(BUILD_SHARED_LIBS)
  add_library(${TargetName} SHARED ${Files})
  target_compile_definitions(${TargetName} PRIVATE PROJECT_TOOL_EXPORT)
else()
  add_library(${TargetName} STATIC ${Files})
  target_compile_definitions(${TargetName} PRIVATE NAU_STATIC_RUNTIME)
endif()

set_target_properties(
  ${TargetName}
  PROPERTIES
  FOLDER "${NauEngineFolder}/tools"
)

target_sources(${TargetName} PRIVATE ${Sources} ${Public})

target_link_libraries(${TargetName} PRIVATE nlohmann_json::nlohmann_json NauKernel Shared)
target_include_directories(${TargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PUBLIC_HEADERS
  "include/nau/project_tools/project_api.h"
  "include/nau/project_tools/project_info.h"
)
set_target_properties(${TargetName} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

nau_add_compile_options(${TargetName})

install(TARGETS ${TargetName} DESTINATION bin/$<CONFIG> PUBLIC_HEADER DESTINATION include/${TargetName})