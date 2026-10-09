
add_subdirectory(lua_toolkit)
add_subdirectory(src)

set_target_properties (
  LuaToolkit
  PROPERTIES
    FOLDER "${NauEngineFolder}/libs"
)

if (NAU_CORE_TESTS)
    nau_collect_cmake_subdirectories(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    foreach(test ${tests})
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/${test})
    endforeach()
endif()