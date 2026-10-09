// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <vector>

#include "iostream"
#include "nau/shared/api.h"
#include "string"

namespace nau
{
    struct CommonArguments
    {
        std::string projectPath;
        std::string toolsPath;
    };

    struct InitProjectArguments : public CommonArguments
    {
        std::string templateName;
        std::string projectName;
        bool contentOnly;
        bool generateSolutionFile;
        bool openIde;
        std::string cMakePreset;
    };

    struct RebuildProjectArguments : public CommonArguments
    {
        bool autoOpenIde;
        std::string cMakePreset;
        std::string projectName;
        bool openIde;
    };

    struct UpgradeProjectArguments : public CommonArguments
    {
        std::string version;
        bool dontUpgrade;
        std::string projectName;
    };

    struct SaveProjectArguments : public CommonArguments
    {
        std::string config;
        bool dontUpgrade;
        std::string projectName;
    };

    struct ImportAssetsArguments : public CommonArguments
    {
        std::string projectPath;
        std::string assetPath;
        std::vector<std::string> filesExtensions;
    };

    struct BuildProjectArguments : public CommonArguments
    {
        std::string projectName;
        std::string config;
        std::string targetDirectory;
        bool openAfterBuild;
        bool compileSources;
        bool compileAssets;
        bool copyBinaries;
        std::string cMakePreset;
    };
}  // namespace nau