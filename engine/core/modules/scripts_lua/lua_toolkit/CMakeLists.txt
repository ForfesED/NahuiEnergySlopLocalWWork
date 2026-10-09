add_subdirectory(src)

if (NAU_CORE_TESTS)
    add_subdirectory(tests/test_lua)
endif()
