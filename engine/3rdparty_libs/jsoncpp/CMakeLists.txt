cmake_minimum_required(VERSION 3.20)

set(TargetName jsoncpp)

nau_collect_files(SOURCE
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  MASK "*.h" "*.cpp"
)

add_library(${TargetName} ${SOURCE})

target_include_directories(${TargetName} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/jsoncpp/include>
)

target_compile_definitions(${TargetName} PUBLIC
  JSON_USE_EXCEPTION=0
)

if (BUILD_SHARED_LIBS)
  target_compile_definitions(${TargetName} PRIVATE
    JSON_DLL_BUILD)

  target_compile_definitions(${TargetName} PUBLIC
    JSON_DLL)
endif()

nau_add_compile_options(${TargetName})

nau_collect_files(HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  MASK "*.h" "*.hpp" "*.ipp"
)

nau_install(${TargetName} 3rdPartyLibs ${HEADERS})