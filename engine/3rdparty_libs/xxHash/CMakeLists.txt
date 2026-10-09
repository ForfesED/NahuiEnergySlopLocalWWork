set(TARGET_NAME xxHash)
project(${TARGET_NAME})

set(SOURCES
  #xxHash/xxh_x86dispatch.c
  xxHash/xxhash.c
)

set(HEADERS
  xxHash/xxh3.h
  xxHash/xxhash.h
  xxHash/xxh_x86dispatch.h
)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

add_library(${TARGET_NAME} STATIC)

target_include_directories(${TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/3rdPartyLibs/xxHash>
)

target_sources(${TARGET_NAME} PRIVATE ${SOURCES} ${HEADERS}) 

nau_install(${TARGET_NAME} 3rdPartyLibs ${HEADERS})