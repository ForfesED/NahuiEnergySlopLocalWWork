add_subdirectory(src)

if (NAU_CORE_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test_assets)
endif()