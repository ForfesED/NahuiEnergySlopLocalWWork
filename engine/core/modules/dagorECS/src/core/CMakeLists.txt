
# CMakeList.txt : CMake project for NAU::gameLibs_daECS_core 
# git: -

set(PUBLIC_HEADERS 
    ../../publicInclude/daECS/core/baseComponentTypes/arrayType.h
    ../../publicInclude/daECS/core/baseComponentTypes/listType.h
    ../../publicInclude/daECS/core/baseComponentTypes/objectType.h
    ../../publicInclude/daECS/core/baseIo.h
    ../../publicInclude/daECS/core/component.h
    ../../publicInclude/daECS/core/componentsMap.h
    ../../publicInclude/daECS/core/componentType.h
    ../../publicInclude/daECS/core/componentTypes.h
    ../../publicInclude/daECS/core/coreEvents.h
    ../../publicInclude/daECS/core/dataComponent.h
    ../../publicInclude/daECS/core/ecsGameRes.h
    ../../publicInclude/daECS/core/ecsHash.h
    ../../publicInclude/daECS/core/ecsQuery.h
    ../../publicInclude/daECS/core/entityComponent.h
    ../../publicInclude/daECS/core/entityId.h
    ../../publicInclude/daECS/core/entityManager.h
    ../../publicInclude/daECS/core/entitySystem.h
    ../../publicInclude/daECS/core/event.h
    ../../publicInclude/daECS/core/internal/archetypes.h
    ../../publicInclude/daECS/core/internal/asserts.h
    ../../publicInclude/daECS/core/internal/bookAllocator.h
    ../../publicInclude/daECS/core/internal/circularBuffer.h
    ../../publicInclude/daECS/core/internal/componentTypeBoxedConstructor.h
    ../../publicInclude/daECS/core/internal/dataComponentManager.h
    ../../publicInclude/daECS/core/internal/ecsCounterRestorer.h
    ../../publicInclude/daECS/core/internal/entityManagerImplementations.h
    ../../publicInclude/daECS/core/internal/entityManagerProtected.h
    ../../publicInclude/daECS/core/internal/eventsDb.h
    ../../publicInclude/daECS/core/internal/fastPacketHashMap.h
    ../../publicInclude/daECS/core/internal/inplaceKeySet.h
    ../../publicInclude/daECS/core/internal/ltComponentList.h
    ../../publicInclude/daECS/core/internal/performQuery.h
    ../../publicInclude/daECS/core/internal/should_process.h
    ../../publicInclude/daECS/core/internal/stackAllocator.h
    ../../publicInclude/daECS/core/internal/templates.h
    ../../publicInclude/daECS/core/internal/trackComponentAccess.h
    ../../publicInclude/daECS/core/internal/typesAndLimits.h
    ../../publicInclude/daECS/core/schemelessEvent.h
    ../../publicInclude/daECS/core/sharedComponent.h
    ../../publicInclude/daECS/core/template.h
    ../../publicInclude/daECS/core/updateStage.h
    ../../publicInclude/daECS/core/utility/ecsForEach.h
    ../../publicInclude/daECS/core/utility/ecsForEachWithArg.h
    ../../publicInclude/daECS/core/utility/enumDescription.h
    ../../publicInclude/daECS/core/utility/enumReflection.h
    ../../publicInclude/daECS/core/utility/enumRegistration.h
    ../../publicInclude/daECS/core/utility/nullableComponent.h
)
set(SOURCE 
    archetypes.cpp
    baseIo.cpp
    component.cpp
    componentType.cpp
    componentTypes.cpp
    dataComponent.cpp
    dataManager.cpp
    entityManager2.cpp
    entityManagerDebug.cpp
    entityManagerEvent.cpp
    entityManagerMT.cpp
    entityManagerQuery.cpp
    entityManagerTrack.cpp
    entityManagerUpdate.cpp
    entitySystem.cpp
    eventsDb.cpp
    legacy.cpp
    schemelessEvent.cpp
    shrinkArchetypes.cpp
    singletonES.cpp
    template2.cpp
    templates.cpp
    updateTemplates.cpp
)
set(HEADERS 
    archetypesInternal.h
    check_es_optional.h
    chunkedVectorMT.h
    dataComponentManagerInt.h
    ecsInternal.h
    ecsPerformQueryInline.h
    ecsQueryInternal.h
    ecsQueryManager.h
    entityCreationProfiler.h
    entityManagerEvent.h
    singletonName.h
    specialized_memcpy.h
    tokenize_const_string.h
)



source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES 
    ${SOURCE}
    ${HEADERS}


) 

source_group("PublicHeaders" FILES     ${PUBLIC_HEADERS}
)

add_library(core STATIC)
add_library(NAU::gameLibs_daECS_core ALIAS core)

target_sources(core PRIVATE 
    ${SOURCE}
    ${HEADERS}

    ${PUBLIC_HEADERS}


)

target_include_directories(core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_include_directories(core PUBLIC 
#     
# #<user-code:extra_includes

# #>
# )

target_link_libraries(core PUBLIC
    NAU::engine2_baseUtil
    NAU::gameLibs_publicInclude
    regExp
) 
#target_link_libraries(core eastl)

# target_include_directories(core PUBLIC 
# 	$<BUILD_INTERFACE:${NAU_ENGINE_ROOT}/prog/engine/dagorInclude>
# 	$<BUILD_INTERFACE:${NAU_ENGINE_ROOT}/prog/gameLibs/publicInclude>

# ) 



target_compile_options(core PRIVATE 
	#    /clang:-fsave-optimization-record  

) 




add_nau_compile_options(core)
#add_nau_folder_property(core LIB gameLibs)

set_target_properties (core PROPERTIES
    FOLDER "${NauEngineFolder}/NAU/gameLibs/daECS/core/"
)

install(TARGETS core
    EXPORT core    
    RUNTIME DESTINATION "bin"  
    ARCHIVE DESTINATION "lib/gameLibs/core"
    LIBRARY DESTINATION "lib/gameLibs/core"
)

if(DEFINED HEADERS)
    install(FILES ${HEADERS}     ${PUBLIC_HEADERS}
 DESTINATION "include/gameLibs/core")
endif()

install(EXPORT core
    FILE core-config.cmake
    NAMESPACE NAUE::
    DESTINATION "lib/cmake/gameLibs/core"
)



