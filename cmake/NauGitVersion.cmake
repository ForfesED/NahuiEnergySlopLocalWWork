# Generate include/nau/version/vcs_version.h from Git repository
# Derived from https://jonathanhamberg.com/post/cmake-embedding-git-hash/ 

set(CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

function(git_read_state git_hash git_branch)
    if (EXISTS ${CMAKE_BINARY_DIR}/gitstate_hash.txt)
        file(STRINGS ${CMAKE_BINARY_DIR}/gitstate_hash.txt CONTENT)
        LIST(GET CONTENT 0 var)
        set(${git_hash} ${var} PARENT_SCOPE)
    endif ()
    if (EXISTS ${CMAKE_BINARY_DIR}/gitstate_branch.txt)
        file(STRINGS ${CMAKE_BINARY_DIR}/gitstate_branch.txt CONTENT)
        LIST(GET CONTENT 0 var)
        set(${git_branch} ${var} PARENT_SCOPE)
    endif ()
endfunction()

function(git_check_version)
    find_package(Git)

    if(NOT DEFINED GIT_HASH)
        if(Git_FOUND)
            # Get the latest commit hash of the working branch
            execute_process(
              COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
              WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
              OUTPUT_VARIABLE GIT_HASH
              OUTPUT_STRIP_TRAILING_WHITESPACE
              ERROR_QUIET 
              )
            if(GIT_HASH STREQUAL "")
                set(GIT_HASH "unknown")
            endif ()
        endif ()
    endif ()

    if(NOT DEFINED GIT_BRANCH)
        if(Git_FOUND)
        # Get the working branch
            execute_process(
              COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
              WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
              OUTPUT_VARIABLE GIT_BRANCH
              OUTPUT_STRIP_TRAILING_WHITESPACE
              ERROR_QUIET 
              )
            if(GIT_BRANCH STREQUAL "")
                set(GIT_BRANCH "unknown")
            endif ()
        endif ()
    endif ()

    set(GIT_HASH_CACHE "unknown")
    set(GIT_BRANCH_CACHE "unknown")
    git_read_state(GIT_HASH_CACHE GIT_BRANCH_CACHE)

    add_compile_definitions(NAU_GIT_COMMIT="${GIT_HASH}")
    add_compile_definitions(NAU_GIT_BRANCH="${GIT_BRANCH}")
endfunction()

# This is used to run this function from an external cmake process.
if (RUN_CHECK_GIT_VERSION)
    git_check_version()
endif()