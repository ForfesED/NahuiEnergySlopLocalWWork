set(TargetName ShaderCompilerTool)

if(${BUILD_SHARED_LIBS})
    set(ExcludeFiles )
else()
    set(ExcludeFiles 
        shader_cache_from_asset.cpp
        shader_cache_from_asset.h
    )
endif()

nau_collect_files(Sources
    DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    MASK "*.cpp" "*.h"
    EXCLUDE ${ExcludeFiles}
)

find_package(directx-dxc CONFIG REQUIRED)

add_executable(${TargetName} ${Sources})

set(DXIL_TOOLS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/directx-dxc")
set(DXIL_DLL "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/dxil.dll")

add_custom_command(TARGET ${TargetName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DXIL_DLL}"
    "$<TARGET_FILE_DIR:${TargetName}>"
)

target_link_libraries(${TargetName} PRIVATE
    Microsoft::DXIL
    Microsoft::DirectXShaderCompiler
    CoreAssets
)

if(${BUILD_SHARED_LIBS})
    target_link_libraries(${TargetName} PRIVATE
        UsdMetaTools
    )
endif()

nau_add_compile_options(${TargetName})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})
set_target_properties(${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/tools"
)

install(FILES ${DXIL_TOOLS}/dxc.exe ${DXIL_TOOLS}/dxcompiler.dll ${DXIL_TOOLS}/dxil.dll  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/$<CONFIG>)

nau_install(${TargetName} tools)