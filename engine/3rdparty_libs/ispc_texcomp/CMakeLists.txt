# CMakeList.txt : CMake project for ispc, include source and define
# https://github.com/GameTechDev/ISPCTextureCompressor.git rev 691513b4fb406eccfc2f7d7f8213c8505ff5b897

project(ispc_texcomp CXX)

function(add_parser_generator_command targetPath outFilesH outFilesObj grammarFile)
    
    cmake_path(GET grammarFile PARENT_PATH parentPath)
    cmake_path(GET grammarFile STEM filename)

    set(expectedOutput
        "${targetPath}/${filename}_ispc.h"
        "${targetPath}/${filename}_ispc_avx2.h"
        "${targetPath}/${filename}_ispc_sse2.h"
        "${targetPath}/${filename}_ispc_sse4.h"
        "${targetPath}/${filename}_ispc_avx.h"

        "${targetPath}/${filename}.obj"
        "${targetPath}/${filename}_avx2.obj"
        "${targetPath}/${filename}_sse2.obj"
        "${targetPath}/${filename}_sse4.obj"
        "${targetPath}/${filename}_avx.obj"
    )

    if (NOT Platform_Windows)
        message (FATAL_ERROR "Please, add support for other development platforms: linux, osx")
    endif()

    add_custom_command(
        OUTPUT ${expectedOutput}
        COMMAND ${parentPath}/ISPC/win/ispc_gen.cmd -h "${targetPath}/${filename}_ispc.h" -o "${targetPath}/${filename}.obj" -O2 --pic --opt=fast-math --woff --target=sse2,sse4,avx,avx2 ${grammarFile}
        DEPENDS ${grammarFile}
        VERBATIM
    )

    set(${outFilesH} ${${outFilesH}}
        "${targetPath}/${filename}_ispc.h"
        "${targetPath}/${filename}_ispc_avx2.h"
        "${targetPath}/${filename}_ispc_sse2.h"
        "${targetPath}/${filename}_ispc_sse4.h"
        "${targetPath}/${filename}_ispc_avx.h"
        PARENT_SCOPE)

    set(${outFilesObj} ${${outFilesObj}}
        "${targetPath}/${filename}.obj"
        "${targetPath}/${filename}_avx2.obj"
        "${targetPath}/${filename}_sse2.obj"
        "${targetPath}/${filename}_sse4.obj"
        "${targetPath}/${filename}_avx.obj"
        PARENT_SCOPE)

endfunction()

set(HEADERS
    ispc_texcomp.h
)

set(SOURCES
    ispc_texcomp.cpp
    ispc_texcomp_astc.cpp
)

set(ISPC
    kernel.ispc
    kernel_astc.ispc
)

set(OBJECTS)

set(GENERATED_PATH "${CMAKE_CURRENT_BINARY_DIR}/Ispc-Generated")

add_parser_generator_command(${GENERATED_PATH} HEADERS OBJECTS "${CMAKE_CURRENT_SOURCE_DIR}/kernel.ispc")
add_parser_generator_command(${GENERATED_PATH} HEADERS OBJECTS "${CMAKE_CURRENT_SOURCE_DIR}/kernel_astc.ispc")

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
source_group("ISPC" FILES ${ISPC})
source_group("Objects" FILES ${OBJECTS})

add_library(ispc_texcomp STATIC)
target_sources(ispc_texcomp PRIVATE ${HEADERS} ${SOURCES} ${ISPC} ${OBJECTS})

target_include_directories(ispc_texcomp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/ispc_texcomp>
)

target_include_directories(ispc_texcomp PRIVATE ${GENERATED_PATH})


SET_SOURCE_FILES_PROPERTIES(
    ${OBJECTS}
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
)

nau_install(ispc_texcomp 3rdPartyLibs)