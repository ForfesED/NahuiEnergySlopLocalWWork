
set(TargetName test_usd_proxy)

set(SOURCES
    main.cpp
)

add_executable(${TargetName} ${SOURCES})
add_test(NAME UsdProxy COMMAND ${TargetName})

target_include_directories(${TargetName} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TargetName} PRIVATE
  gtest
  gmock
  UsdProxy
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
set_target_properties (${TargetName} PROPERTIES
    FOLDER "Editor/tests"
)


if (WIN32)
    set(CONFIG_PLUGIN_PATH "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>$<$<NOT:$<CONFIG:DEBUG>>:${CMAKE_BINARY_DIR}/bin/Release>")
    
    set_target_properties(${TargetName} 
        PROPERTIES VS_DEBUGGER_ENVIRONMENT 
        "PATH=${PXR_ENV_PATHS};${PXR_DEBUGER_ENV};
        ${PXR_DEBUGER_PLUGINPATH}${CONFIG_PLUGIN_PATH}/plugins;")
endif()


# todo manage test env 
include(GoogleTest)
#gtest_discover_tests(${TargetName}
#    PROPERTIES
#    ENVIRONMENT_MODIFICATION "PATH=${PXR_ENV_PATHS};%PATH%;${PXR_DEBUGER_ENV};${PXR_DEBUGER_PLUGINPATH};${PXR_DEBUGER_PLUGINPATH}"
#)