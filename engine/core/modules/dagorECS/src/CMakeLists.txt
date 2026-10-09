
set(TargetName DagorECS)
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH moduleRoot)

nau_collect_files(Sources
    RELATIVE ${moduleRoot}/src
    DIRECTORIES ${moduleRoot}/src
    MASK "*.cpp" "*.inl"
    EXCLUDE
    "/platform/.*"
    "net/.*"
)

nau_collect_files(Headers
    RELATIVE ${moduleRoot}
    DIRECTORIES ${moduleRoot}/include
    MASK "*.h"
    PREPEND "../"
)
#if (${Platform_Windows})
#
#  nau_collect_files(Sources
#    DIRECTORIES 
#      ${moduleRoot}/src/platform/windows
#      ${moduleRoot}/include/platform/windows
#    RELATIVE ${moduleRoot}
#    MASK "*.cpp" "*.h"
#  )
#endif()

nau_add_module(${TargetName}
    SOURCES ${Sources} ${Headers}
    FOLDER "${NauEngineFolder}/core/modules"
    EXPORT_AS_LIBRARY
)

target_precompile_headers(${TargetName} PRIVATE pch.h)
target_include_directories(${TargetName} PRIVATE
    ${moduleRoot}/src
)
target_compile_definitions(${TargetName} PRIVATE NAU_DAGORECS_BUILD)
target_compile_definitions(${TargetName} PUBLIC
    _TARGET_PC=1
    _TARGET_PC_WIN=1
    _TARGET_64BIT=1
    _TARGET_SIMD_SSE=1
)

source_group(TREE ${moduleRoot}/src PREFIX Sources FILES ${Sources})
source_group(TREE ${moduleRoot}/include PREFIX Headers FILES ${Headers})

target_link_libraries(${TargetName} PUBLIC
    ska_hash_map
    xxHash
)

target_include_directories(${TargetName} PUBLIC
    ${moduleRoot}/include
)
