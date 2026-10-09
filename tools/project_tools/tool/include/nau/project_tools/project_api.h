// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/shared/args.h"
#include "nau/shared/version.h"

#ifndef NAU_STATIC_RUNTIME

    #ifdef _MSC_VER
        #ifdef PROJECT_TOOL_EXPORT
            #define PROJECT_TOOL_API __declspec(dllexport)
        #else
            #define PROJECT_TOOL_API __declspec(dllimport)
        #endif

    #else
        #error Unknown Compiler/OS
    #endif

#else
    #define PROJECT_TOOL_API
#endif

namespace nau
{
    class Logger;
    struct ProjectInfo;

    int PROJECT_TOOL_API initProject(const InitProjectArguments* args);
    int PROJECT_TOOL_API rebuildProject(const RebuildProjectArguments* args);
    int PROJECT_TOOL_API upgradeProject(const UpgradeProjectArguments* args);
    int PROJECT_TOOL_API saveProject(const SaveProjectArguments* args);
    std::shared_ptr<ProjectInfo> PROJECT_TOOL_API loadProject(const std::string& path);
    bool PROJECT_TOOL_API unloadProject(const std::string& path);
    NauVersion PROJECT_TOOL_API getVersion();
} 