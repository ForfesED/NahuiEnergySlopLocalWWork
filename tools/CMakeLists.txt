
if (NAU_CORE_TOOLS)

  nau_collect_cmake_subdirectories(toolsDirs ${CMAKE_CURRENT_SOURCE_DIR})

  foreach(tool ${toolsDirs})
    message(STATUS "Found tool: (${tool})")
    add_subdirectory(${tool})
  endforeach()

else()
	if (NAU_FORCE_ENABLE_SHADER_COMPILER_TOOL)
		add_subdirectory(shader_compiler_tool)
	endif()
endif()