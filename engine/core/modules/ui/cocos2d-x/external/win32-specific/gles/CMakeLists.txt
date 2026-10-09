cmake_minimum_required(VERSION 3.6)

set(lib_name gles)
set(target_name ext_${lib_name})

project(${lib_name})

include(../../cmake/CocosExternalConfig.cmake)

add_library(${target_name} SHARED IMPORTED GLOBAL)

set_target_properties(${target_name} PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include/OGLES
)

if (NOT Platform_Windows)
  message(FATAL_ERROR "Use gles only on Windows platform")
endif()

set(shared_lib_fname "glew32.dll")
set(shared_lib_path "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/x64/${shared_lib_fname}")

set_target_properties(${target_name} PROPERTIES
  IMPORTED_LOCATION "${shared_lib_path}"
  IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/x64/glew32.lib"
)

install(FILES ${shared_lib_path} 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/x64/glew32.lib"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/x64/glew32.lib"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  RENAME ${target_name}.lib)
