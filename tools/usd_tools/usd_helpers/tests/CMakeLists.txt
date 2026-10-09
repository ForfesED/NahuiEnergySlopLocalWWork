set(TargetName test_usd_helpers)

set(SOURCES
    main.cpp
    test_usd_wrapper.cpp
)

add_executable(${TargetName} ${SOURCES})
add_test(NAME UsdHelpers COMMAND ${TargetName})

target_include_directories(${TargetName} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TargetName} PRIVATE
    gtest
    gmock

    usd

    UsdHelpers
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

if (WIN32)
    set_target_properties(${TargetName} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${PXR_ENV_PATHS};%PATH%;${PXR_DEBUGER_ENV};${PXR_DEBUGER_PLUGINPATH}")
endif()


# todo manage test env
#include(GoogleTest)
#gtest_discover_tests(${TargetName}
#    PROPERTIES
#    ENVIRONMENT_MODIFICATION "PATH=${PXR_ENV_PATHS};%PATH%;${PXR_DEBUGER_ENV};${PXR_DEBUGER_PLUGINPATH}"
#)
