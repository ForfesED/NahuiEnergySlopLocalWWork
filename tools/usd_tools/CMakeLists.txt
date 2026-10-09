if(BUILD_SHARED_LIBS)

    add_custom_target( usd-plugins )
    set(CONFIG_PLUGIN_PATH "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>$<$<NOT:$<CONFIG:DEBUG>>:${CMAKE_BINARY_DIR}/bin/Release>")
    add_custom_command(TARGET usd-plugins POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/plugInfo.json" "${CONFIG_PLUGIN_PATH}/plugins/plugInfo.json"
    )

    add_subdirectory(usd_proxy)
    add_subdirectory(usd_translator)
    add_subdirectory(usd_helpers)
    add_subdirectory(plugins)
    add_subdirectory(usd_meta_tools)

    set_target_properties (
        UsdProxy 
        UsdTranslator 
        UsdTranslatorWrapper 
        UsdMetaTools
        UsdUidLookup
    PROPERTIES
        FOLDER "${NauEngineFolder}/tools/usd"
    )

    set_target_properties (
        usd-plugins 
        NauAssetMetadata 
        NauGuiSchema 
        NauAnimationClipAsset 
        NauPhysicsSchema 
        NauAudioSchema 
        NauVFXSchema 
        NauComponentSchema 
        NauInputSchema 
        NauUsdFormat 
    PROPERTIES
        FOLDER "${NauEngineFolder}/tools/usd/plugins"
    )

    if (NAU_CORE_TESTS)
        set_target_properties (
            test_usd_translator 
            test_usd_proxy 
            test_usd_helpers
        PROPERTIES
            FOLDER "${NauEngineFolder}/tools/usd/tests"
        )
    endif()

endif()
