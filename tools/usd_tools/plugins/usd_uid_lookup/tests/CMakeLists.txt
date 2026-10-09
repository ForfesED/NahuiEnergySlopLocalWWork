set(TargetName "test_usd_uid_lookup")

set(Sources
    usd_uid_lookup_tests.cpp
    usd_uid_lookup_tests_main.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})
add_executable(${TargetName} ${Sources})
add_test(NAME UsdUidLookup COMMAND ${TargetName})

target_include_directories(${TargetName} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TargetName} PRIVATE
    NauKernel
    Shared
    nlohmann_json::nlohmann_json
    gtest
    gmock
    UsdUidLookup
    usd
    UsdProxy
)

nau_add_compile_options(${TargetName})

if (WIN32)
    set(CONFIG_PLUGIN_PATH "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>$<$<NOT:$<CONFIG:DEBUG>>:${CMAKE_BINARY_DIR}/bin/Release>")
    
    set_target_properties(${TargetName} 
        PROPERTIES VS_DEBUGGER_ENVIRONMENT 
            "PATH=${PXR_ENV_PATHS};${PXR_DEBUGER_ENV};
            PXR_PLUGINPATH_NAME=${CONFIG_PLUGIN_PATH}/plugins;")
endif()

#include(GoogleTest)
#gtest_discover_tests(${TargetName})