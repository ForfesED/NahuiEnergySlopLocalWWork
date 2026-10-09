set(TargetName Shared)

nau_collect_files(Sources
    ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
    DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
    MASK "*.cpp"
)

nau_collect_files(Headers
    ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
    DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
    MASK "*.h"
)

set(Files ${Sources} ${Headers})

if(BUILD_SHARED_LIBS)
    add_library(${TargetName} SHARED ${Files})
    target_compile_definitions(${TargetName} PRIVATE SHARED_EXPORT)
else()
    add_library(${TargetName} STATIC ${Files})
    target_compile_definitions(${TargetName} PRIVATE NAU_STATIC_RUNTIME)
endif()


target_link_libraries(${TargetName} PRIVATE
    nlohmann_json::nlohmann_json
    NauKernel
    usd
)

nau_add_compile_options(${TargetName})

target_include_directories(${TargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})
set_target_properties(${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/tools"
)

set(PUBLIC_HEADERS
    "include/nau/shared/args.h"
    "include/nau/shared/error_codes.h"
    "include/nau/shared/file_system.h"
    "include/nau/shared/logger.h"
    "include/nau/shared/macro.h"
    "include/nau/shared/util.h"
    "include/nau/shared/version.h"
)

set_target_properties(${TargetName} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

install(TARGETS ${TargetName} DESTINATION bin/$<CONFIG> PUBLIC_HEADER DESTINATION include/${TargetName})