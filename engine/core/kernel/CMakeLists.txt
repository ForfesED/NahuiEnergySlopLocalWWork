add_subdirectory(src)

if (NAU_CORE_TESTS)

    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests/test_common")

    nau_collect_cmake_subdirectories(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    foreach(test ${tests})
        if (NOT ${test} STREQUAL "test_common")
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/${test})
        endif()
    endforeach()
endif()
