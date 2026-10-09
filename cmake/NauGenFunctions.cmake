set(ES_SCRIPTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/cmake/_scripts)
function(nau_check_python)
    macro(check_package package)
        execute_process(
            COMMAND pip show ${package}
            RESULT_VARIABLE EXIT_CODE
            OUTPUT_QUIET
        )

        if(NOT ${EXIT_CODE} EQUAL 0)
            message(
                FATAL_ERROR
                "The \"${package}\" Python3 package is not installed. Please install it using the following command: \"pip3 install ${package}\"."
            )
        endif()
    endmacro()

    check_package(cymbal)
    check_package(clang)
endfunction()


###     nau_gen_es(
###         <target_name>
###         <out-var> # list of *ES.cpp.gen.es.cpp files
###         <out-var> # pull file
###         [PULL_NAME pull_name (default ${target}_pull)]
###         [<*ES.cpp.inl>...]
###    )
###
###  Generates *ES.cpp.gen.es.cpp files from *ES.cpp.inl files, and returns the paths to this files. It also generates and returns lib_es_pull.cpp file
function(nau_gen_es target esFilesOut esPullFileOut)
    set(oneValueArgs PULL_NAME)
    cmake_parse_arguments(COLLECT "" "${oneValueArgs}" "" ${ARGN})

    set(PULL_NAME "${target}_pull")
    if(DEFINED COLLECT_PULL_NAME)
        set(ARGN ${COLLECT_UNPARSED_ARGUMENTS})
        set(PULL_NAME "${COLLECT_PULL_NAME}")
    endif()

    set(esNames "")
    set(esFiles "")
    set(esGenFiles "")

    foreach(file ${ARGN})
        string(FIND ${file} "ES.cpp.inl" nameEnd)
        if(nameEnd STREQUAL "-1")
            string(FIND ${file} ".cpp.inl" nameEnd)
        endif()
        string(FIND ${file} "/" nameStart REVERSE)
        math(EXPR nameStart "${nameStart} + 1")
        math(EXPR len "${nameEnd} - ${nameStart}")
        string(SUBSTRING ${file} ${nameStart} ${len} esFileName)
        set(esNames ${esNames} "${esFileName}")

        string(REPLACE ".cpp.inl" ".cpp.gen.es.cpp" esFile ${file})
        list(APPEND esGenFiles "${CMAKE_CURRENT_BINARY_DIR}/${esFile}")
        list(APPEND esFiles "${CMAKE_CURRENT_LIST_DIR}/${file}")
    endforeach(file)

    set(${esFilesOut} "" PARENT_SCOPE)
    set(${esPullFileOut} "" PARENT_SCOPE)

    target_true_name(${target} targetTrueName)

    set(es_arguments
        TARGET ${targetTrueName}
        PULL_NAME ${PULL_NAME}
        PULL_FILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/${target}.lib_es_pull.cpp
        ES_FILES ${esFiles}
        ES_FILES_OUT ${esGenFiles}
    )
    list(JOIN es_arguments "*" es_arguments)
    set_property(GLOBAL APPEND PROPERTY PROPERTY_NAU_GEN_ES_PARAMS_ARRAY ${es_arguments})
endfunction()

function(_nau_get_target_property target param out)
    get_target_property(targetParam ${target} ${param})
    if("${targetParam}" STREQUAL targetParam-NOTFOUND)
        set(targetParam "")
    endif()
    set(${out} ${targetParam} PARENT_SCOPE)
endfunction(_nau_get_target_property)

function(target_true_name targetName targetTrueName)
    get_target_property(aliased_name ${targetName} ALIASED_TARGET)
    if(aliased_name STREQUAL aliased_name-NOTFOUND)
        set(aliased_name ${targetName})
    endif()
    set(${targetTrueName} ${aliased_name} PARENT_SCOPE)
endfunction()

function(_nau_calculate_target_compile_params target)
    get_target_property(targetVisited ${target} COMPILE_DEFINITIONS_NAU_ES_VISITED)

    if("${targetVisited}" STREQUAL START_VISIT)
        message(FATAL_ERROR "Found cycle dependency of ${target} in ${target};${ARGN}.")
    endif()

    if(NOT "${targetVisited}" STREQUAL targetVisited-NOTFOUND)
        return()
    endif()

    target_true_name(${target} targetTrueName)

    set_target_properties(${targetTrueName} PROPERTIES
        COMPILE_DEFINITIONS_NAU_ES_VISITED START_VISIT
    )

    _nau_get_target_property(${target} INTERFACE_LINK_LIBRARIES targetDependencies)
    _nau_get_target_property(${target} LINK_LIBRARIES targetPrivateDependencies)


    foreach(library IN LISTS targetDependencies targetPrivateDependencies)
        if((NOT TARGET ${library}) OR (${library} MATCHES [[([A-Za-z0-9_]+_es_gen|[A-Za-z0-9_]+_es_pulls)]]))
            continue()
        endif()

        _nau_calculate_target_compile_params(${library} ${target} ${ARGN})

        get_target_property(libraryDefinitions ${library} COMPILE_DEFINITIONS_NAU_ES)
        if("${libraryDefinitions}" STREQUAL libraryDefinitions-NOTFOUND)
            message(FATAL_ERROR "Couldn't calculate es compile params for target: ${library}.")
        endif()

        get_target_property(libraryIncludes ${library} INCLUDE_DIRECTORIES_NAU_ES)
        if("${libraryIncludes}" STREQUAL libraryIncludes-NOTFOUND)
            message(FATAL_ERROR "Couldn't calculate es compile params for target: ${library}.")
        endif()

        list(APPEND targetParentDefinitions ${libraryDefinitions})
        list(APPEND targetParentIncludes ${libraryIncludes})

        list(REMOVE_DUPLICATES targetParentDefinitions)
        list(REMOVE_DUPLICATES targetParentIncludes)
    endforeach(library)

    _nau_get_target_property(${target} INTERFACE_INCLUDE_DIRECTORIES targetIncludes)
    _nau_get_target_property(${target} INTERFACE_COMPILE_DEFINITIONS targetDefinitions)

    list(APPEND targetDefinitions ${targetParentDefinitions})
    list(APPEND targetIncludes ${targetParentIncludes})

    get_target_property(targetSourceDir ${target} SOURCE_DIR)
    set(includes)
    set(targetTrueIncludes)

    string(REPLACE "SEMICOLON" ">;<" targetIncludes "${targetIncludes}")
    string(REPLACE "$<>" "" targetIncludes "${targetIncludes}")
    string(REPLACE "<>" "" targetIncludes "${targetIncludes}")
    foreach(include ${targetIncludes})
        if(include MATCHES [[\$<IN]])
            continue()
        endif()

        string(REPLACE "$<BUILD_INTERFACE:" "" include ${include})
        string(REPLACE ">" "" include ${include})
        if(NOT IS_ABSOLUTE ${include})
            set(include ${targetSourceDir}/${include})
        endif()
        set(targetTrueIncludes ${targetTrueIncludes} ${include})
    endforeach(include)

    while(targetDefinitions MATCHES [[\$\<[^<:]*\:[^:>]*\>]])
        string(REGEX REPLACE [[\$\<[^<:]*\:[^:>]*\>]] "" targetDefinitions "${targetDefinitions}")
    endwhile()

    list(REMOVE_DUPLICATES targetDefinitions)
    list(REMOVE_DUPLICATES targetTrueIncludes)

    set_target_properties(${targetTrueName}
        PROPERTIES
        COMPILE_DEFINITIONS_NAU_ES "${targetDefinitions}"
        INCLUDE_DIRECTORIES_NAU_ES "${targetTrueIncludes}"
        COMPILE_DEFINITIONS_NAU_ES_VISITED VISITED
    )
endfunction(_nau_calculate_target_compile_params)

function(nau_generate_es_targets)
    get_property(_NAU_GEN_ES_PARAMS_ARRAY GLOBAL PROPERTY PROPERTY_NAU_GEN_ES_PARAMS_ARRAY)
    foreach(es_arguments IN LISTS _NAU_GEN_ES_PARAMS_ARRAY)
        string(REPLACE "*" ";" es_arguments ${es_arguments})
        set(oneValueArgs TARGET PULL_FILE_OUT PULL_NAME)
        set(multiValueArgs ES_FILES ES_FILES_OUT)
        cmake_parse_arguments("ES" "" "${oneValueArgs}" "${multiValueArgs}" ${es_arguments})

        add_library(${ES_TARGET}_es_gen STATIC ${ES_ES_FILES_OUT} ${ES_PULL_FILE_OUT})

        nau_add_compile_options(${ES_TARGET}_es_gen)

        _nau_get_target_property(${ES_TARGET} INTERFACE_LINK_LIBRARIES esTargetDependencies)
        _nau_get_target_property(${ES_TARGET} LINK_LIBRARIES esTargetPrivateDependencies)

        target_link_libraries(${ES_TARGET}_es_gen PUBLIC ${esTargetDependencies})
        target_link_libraries(${ES_TARGET}_es_gen PRIVATE ${esTargetPrivateDependencies})

        get_target_property(type ${ES_TARGET} TYPE)
        if(${type} STREQUAL "INTERFACE_LIBRARY")
            target_link_libraries(${ES_TARGET} INTERFACE ${ES_TARGET}_es_gen)
        else()
            target_link_libraries(${ES_TARGET} PUBLIC ${ES_TARGET}_es_gen)
        endif()



        #add_nau_compile_options(${ES_TARGET}_es_gen)

        #install(TARGETS ${ES_TARGET}_es_gen
        #    EXPORT ${ES_TARGET}_es_gen
        #    RUNTIME DESTINATION "bin"
        #    ARCHIVE DESTINATION "lib/esGenerated/${ES_TARGET}_es_gen"
        #    LIBRARY DESTINATION "lib/esGenerated/${ES_TARGET}_es_gen"
        #)

        #install(EXPORT ${ES_TARGET}_es_gen
        #    FILE ${ES_TARGET}_es_gen-config.cmake
        #    NAMESPACE NAUE::
        #    DESTINATION "lib/cmake/esGenerated/${ES_TARGET}_es_gen"
        #)

        set(disabelWarnings "/wd4514" "/wd4061" "/wd4820" "/wd4668" "/wd4619" "/wd4365" "/wd4127" "/wd4302" "/wd4242" "/wd4244" "/wd4265" "/wd4101" "/wd4201" "/wd4625" "/wd4626" "/wd4800" "/wd4018" "/wd4710" "/wd4245" "/wd4291" "/wd4389" "/wd4200" "/wd4255" "/wd4711" "/wd4062" "/wd4355" "/wd4640" "/wd4305" "/wd4324" "/wd4511" "/wd4512" "/wd4305" "/wd4738" "/wd4996" "/wd4005" "/wd4740" "/wd4702" "/wd4826" "/wd4503" "/wd4748" "/wd4987" "/wd4574" "/wd4554" "/wd4471" "/wd4350" "/wd4370" "/wd4371" "/wd4316" "/wd4388" "/wd4091" "/wd5026" "/wd5027" "/wd4774" "/wd4312" "/wd4302" "/wd4334" "/wd5220" "/wd5219" "/wd4464" "/wd4463" "/wd4589" "/wd4595" "/wd4435" "/wd4319" "/wd4311" "/wd4267" "/wd4477" "/wd4777" "/wd4548" "/wd5039" "/wd5045" "/wd4623" "/wd5038" "/wd4768" "/wd4456" "/wd5052" "/wd5204" "/wd4577" "/wd4866" "/wd5245" "/wd5246" "/wd5264")

        set(CXXOptions "-X" "/c" "/nologo" "/Zc:forScope" "/Zp8" "/J" "/Zc:inline" "/bigobj" "/Zc:wchar_t" ${CXX_STANDARD} "/Od" "/RTC1" "/EHsc" "/GR-" "-MT" "/FS" "-Wall" "-WX" "/permissive-" "-fp:fast" "-TP")

        _nau_calculate_target_compile_params(${ES_TARGET})

        _nau_get_target_property(${ES_TARGET} INCLUDE_DIRECTORIES targetPrivateIncludes)
        _nau_get_target_property(${ES_TARGET} COMPILE_DEFINITIONS targetPrivateDefinitions)

        get_target_property(targetDefinitions ${ES_TARGET} COMPILE_DEFINITIONS_NAU_ES)
        if("${targetDefinitions}" STREQUAL targetDefinitions-NOTFOUND)
            message(FATAL_ERROR "Couldn't calculate es compile params for target: ${ES_TARGET}.")
        endif()

        get_target_property(targetIncludes ${ES_TARGET} INCLUDE_DIRECTORIES_NAU_ES)
        if("${targetIncludes}" STREQUAL targetIncludes-NOTFOUND)
            message(FATAL_ERROR "Couldn't calculate es compile params for target: ${ES_TARGET}.")
        endif()

        set(targetDefinitions ${targetDefinitions} ${targetPrivateDefinitions})
        set(targetIncludes ${targetIncludes} ${targetPrivateIncludes})

        while(targetDefinitions MATCHES [[\$\<[^<:]*\:[^:>]*\>]])
            string(REGEX REPLACE [[\$\<[^<:]*\:[^:>]*\>]] "" targetDefinitions "${targetDefinitions}")
        endwhile()

        list(REMOVE_DUPLICATES targetDefinitions)
        list(REMOVE_DUPLICATES targetIncludes)

        get_target_property(folder ${ES_TARGET} FOLDER)
        set_target_properties(${ES_TARGET}_es_gen PROPERTIES FOLDER "${folder}")

        target_include_directories(${ES_TARGET}_es_gen PRIVATE ${targetIncludes})

        set(includes)
        foreach(include ${targetIncludes})
            string(FIND ${include} "$<IN" find)
            if(NOT find EQUAL -1)
                continue()
            endif()
            if(DEFINED include)
                set(includes ${includes} "-I${include}")
            endif()
        endforeach(include)

        set(defines)
        foreach(define ${targetDefinitions})
            if(define MATCHES [[(\$\<|\>)]] OR define STREQUAL "")
                continue()
            endif()
            string(REPLACE "/D" "" define ${define})
            string(REPLACE "-D" "" define ${define})

            set(defines ${defines} "-D${define}")
        endforeach(define)

        target_compile_definitions(${ES_TARGET}_es_gen PRIVATE ${defines} -DUSE_BULLET_PHYSICS)

        set(esNames)
        foreach(file fileOut IN ZIP_LISTS ES_ES_FILES ES_ES_FILES_OUT)
            string(FIND ${file} "ES.cpp.inl" nameEnd)
            if(nameEnd STREQUAL "-1")
                string(FIND ${file} ".cpp.inl" nameEnd)
            endif()
            string(FIND ${file} "/" nameStart REVERSE)
            math(EXPR nameStart "${nameStart} + 1")
            math(EXPR len "${nameEnd} - ${nameStart}")
            string(SUBSTRING ${file} ${nameStart} ${len} esFileName)
            set(esNames ${esNames} "${esFileName}")
            get_filename_component(inlFileName ${file} NAME)

            add_custom_command(OUTPUT ${fileOut}
                DEPENDS ${file}
                COMMAND ${Python_EXECUTABLE}
                "\"${ES_SCRIPTS_ROOT}/gen_es.py\""
                "\"${file}\""
                "\"${fileOut}\""
                "\"${inlFileName}\""
                ${CXXOptions}
                ${defines}
                ${disabelWarnings}
                ${includes}
                WORKING_DIRECTORY ${ES_SCRIPTS_ROOT}
            )
        endforeach(file)

        set(${pullFile} ${esPullFile} PARENT_SCOPE)
        add_custom_command(OUTPUT ${ES_PULL_FILE_OUT}
            DEPENDS ${ES_ES_FILES}
            COMMAND ${Python_EXECUTABLE}
            "\"${ES_SCRIPTS_ROOT}/make_es_pull_cpp.py\""
            "\"${ES_PULL_FILE_OUT}\""
            daECS/core/componentType.h
            "\"${ES_PULL_NAME}\""
            ${esNames}
        )
    endforeach()
endfunction(nau_generate_es_targets)

###     nau_stringify_das(
###         <out-var> list of *.das.inl files
###         <stringifyType> type of generation may be "", "--array", "--full-string"
###         [<*.das>...] das files
###    )
###
###  Generates *.das.inl files from *.das files, and returns the paths to this files.
function(nau_stringify_das inlFilesOut stringifyType)
    foreach(file ${ARGN})
        add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/${file}.inl
            DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${file}
            COMMAND ${Python_EXECUTABLE}
            "${ES_SCRIPTS_ROOT}/stringify.py"
            "${stringifyType}"
            "${CMAKE_CURRENT_LIST_DIR}/${file}"
            "${CMAKE_CURRENT_LIST_DIR}/${file}.inl"
            WORKING_DIRECTORY ${NAU_ENGINE_ROOT}/prog/_jBuild/
        )

        list(APPEND genFiles ${CMAKE_CURRENT_LIST_DIR}/${file}.inl)

    endforeach(file)
    set(${inlFilesOut} ${genFiles} PARENT_SCOPE)
endfunction()

# TODO: remove nau_stringify_das and use only nau_stringify_files ?
function(nau_stringify_files inlFilesOut)
    foreach(file ${ARGN})
        add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/${file}.inl
            DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${file}
            COMMAND
            ${Python_EXECUTABLE} ${ES_SCRIPTS_ROOT}/stringify.py
            ${CMAKE_CURRENT_LIST_DIR}/${file}
            ${CMAKE_CURRENT_LIST_DIR}/${file}.inl
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )

        list(APPEND genFiles ${CMAKE_CURRENT_LIST_DIR}/${file}.inl)
    endforeach(file)
    set(${inlFilesOut} ${genFiles} PARENT_SCOPE)
endfunction()
