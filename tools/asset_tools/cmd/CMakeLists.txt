set(TargetName "AssetToolCmd")

nau_collect_files(Sources
    ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}/src
    DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/src
    MASK "*.cpp" "*.h"
)

add_executable(${TargetName})

target_sources(${TargetName}
    PRIVATE
    ${Sources}
)

set_target_properties(${TargetName}
    PROPERTIES
    FOLDER "${NauEngineFolder}/tools"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})

target_link_libraries(${TargetName}
    PRIVATE
    nlohmann_json::nlohmann_json
    argparse::argparse
    AssetTool::AssetTool
    Shared
)

if(BUILD_SHARED_LIBS)
    set_target_properties(${TargetName} PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/$<CONFIG>/AssetTool.lib)
    set_target_properties(${TargetName} PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/$<CONFIG>/Shared.lib)
endif()

if(WIN32)
    set(CONFIG_PLUGIN_PATH "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>$<$<NOT:$<CONFIG:DEBUG>>:${CMAKE_BINARY_DIR}/bin/Release>")

    set_target_properties(${TargetName}
        PROPERTIES VS_DEBUGGER_ENVIRONMENT
        "PATH=${CONFIG_PLUGIN_PATH}/plugins;${PXR_ENV_PATHS};${PXR_DEBUGER_ENV};
        ${PXR_DEBUGER_PLUGINPATH}${CONFIG_PLUGIN_PATH}/plugins;")
endif()

nau_add_compile_options(${TargetName})

install(TARGETS ${TargetName}
    DESTINATION bin/$<CONFIG>
)

