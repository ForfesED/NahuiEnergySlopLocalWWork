// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/shared/args.h"
#include "nau/shared/version.h"

#ifndef NAU_STATIC_RUNTIME

    #ifdef _MSC_VER
        #ifdef ASSET_TOOL_EXPORT
            #define ASSET_TOOL_API __declspec(dllexport)
        #else
            #define ASSET_TOOL_API __declspec(dllimport)
        #endif

    #else
        #error Unknown Compiler/OS
    #endif

#else
    #define ASSET_TOOL_API
#endif

namespace nau
{
    ASSET_TOOL_API int importAssets(const struct ImportAssetsArguments* args);
    ASSET_TOOL_API std::string getCompiledTargetExtensionForType(const std::string& type);
} 