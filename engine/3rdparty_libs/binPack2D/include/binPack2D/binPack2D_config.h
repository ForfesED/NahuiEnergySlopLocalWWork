#pragma once

#if !defined(NAU_STATIC_RUNTIME)

    #ifdef _MSC_VER
        #ifdef NAU_BINPACK_BUILD
            #define NAU_BINPACK_EXPORT __declspec(dllexport)
        #else
            #define NAU_BINPACK_EXPORT __declspec(dllimport)
        #endif

    #else
        #error Unknown Compiler/OS
    #endif

#else
    #define NAU_BINPACK_EXPORT
#endif
