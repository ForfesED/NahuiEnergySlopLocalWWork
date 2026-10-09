macro(cocos2dx_depend)
    # confim the libs, prepare to link
    set(PLATFORM_SPECIFIC_LIBS)

    if(WINDOWS)
        list(APPEND PLATFORM_SPECIFIC_LIBS ws2_32 userenv psapi winmm Version Iphlpapi opengl32)
    endif()
endmacro()

macro(use_cocos2dx_libs_depend target)
    cocos2dx_depend()
    foreach(platform_lib ${PLATFORM_SPECIFIC_LIBS})
      target_link_libraries(${target} PRIVATE ${platform_lib})
    endforeach()
endmacro()

