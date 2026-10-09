# CMakeList.txt : CMake project for ska_hash_map, headeronly
# git:

project(ska_hash_map)

add_library(ska_hash_map INTERFACE)

set(HEADERS
    ska_hash_map/bytell_hash_map.hpp
    ska_hash_map/flat_hash_map.hpp
    ska_hash_map/flat_hash_map2.hpp
    ska_hash_map/unordered_map.hpp
)

source_group("Headers" FILES ${HEADERS})
target_sources(ska_hash_map PRIVATE ${HEADERS})

target_include_directories(ska_hash_map INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/ska_hash_map>
)

nau_install(ska_hash_map 3rdPartyLibs ${HEADERS})
