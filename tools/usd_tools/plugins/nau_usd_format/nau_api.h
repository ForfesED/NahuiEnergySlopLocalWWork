// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#ifndef NAU_STATIC_RUNTIME

    #ifdef _MSC_VER
        #ifdef USD_FORMAT_EXPORT
            #define USD_FORMAT_API __declspec(dllexport)
        #else
            #define USD_FORMAT_API __declspec(dllimport)
        #endif

    #else
        #error Unknown Compiler/OS
    #endif

#else
    #define USD_FORMAT_API
#endif

#define NAUTF_REGISTRY_FUNCTION_NAMED(name)\
static void _Tf_RegistryFunction##name(TfType*, void*);                                                               \
static void _Tf_RegistryAdd##name(TfType*);                                                                           \
namespace                                                                                                             \
{                                                                                                                     \
    __declspec(allocate(".pxrctor")) extern const Arch_ConstructorEntry arch_ctor__Tf_RegistryAdd##name =             \
    {                                                                                                                 \
        reinterpret_cast<Arch_ConstructorEntry::Type>(&_Tf_RegistryAdd##name),                                        \
        static_cast<unsigned>(2408),                                                                                  \
        100                                                                                                           \
    };                                                                                                                \
}                                                                                                                     \
static Arch_PerLibInit<Arch_ConstructorInit> _archCtorInit21;                                                         \
static void _Tf_RegistryAdd##name(TfType*)                                                                            \
{                                                                                                                     \
    Tf_RegistryInit::Add("MFB_ALT_PACKAGE_NAME", (void(*)(TfType*, void*)) _Tf_RegistryFunction##name, "TfType");     \
}                                                                                                                     \
static Arch_PerLibInit<Tf_RegistryStaticInit> _tfRegistryInit##name;                                                  \
static void _Tf_RegistryFunction##name(TfType*, void*)                                                                \


#define NAUAR_DEFINE_RESOLVER(name, base)                                                                            \
static void _Tf_RegistryFunction##name(TfType*, void*);                                                              \
static void _Tf_RegistryAdd##name(TfType*);                                                                          \
namespace                                                                                                            \
{                                                                                                                    \
    __declspec(allocate(".pxrctor")) extern const Arch_ConstructorEntry arch_ctor__Tf_RegistryAdd##name =            \
    {                                                                                                                \
        reinterpret_cast<Arch_ConstructorEntry::Type>(&_Tf_RegistryAdd##name), static_cast<unsigned>(2408), 100      \
    };                                                                                                               \
}                                                                                                                    \
static Arch_PerLibInit<Arch_ConstructorInit> _archCtorInit45;                                                        \
static void _Tf_RegistryAdd##name(TfType*)                                                                           \
{                                                                                                                    \
    Tf_RegistryInit::Add("MFB_ALT_PACKAGE_NAME", (void(*)(TfType*, void*)) _Tf_RegistryFunction##name, "TfType");    \
}                                                                                                                    \
static Arch_PerLibInit<Tf_RegistryStaticInit> _tfRegistryInit46;                                                     \
static void _Tf_RegistryFunction##name(TfType*, void*)                                                               \
{                                                                                                                    \
    Ar_DefineResolver<name, base>();                                                                                 \
};

#define NAU_TOUCH_REGISTRY(name) &arch_ctor__Tf_RegistryAdd##name