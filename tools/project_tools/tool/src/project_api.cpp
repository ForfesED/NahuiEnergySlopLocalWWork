// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/project_tools/project_api.h"

#include <filesystem>
#include <sstream>
#include <string>
#include <iostream>

#include "nau/shared/macro.h"
#include "nau/shared/file_system.h"
#include "nau/shared/logger.h"

#include "nau/project_tools/project_api.h"
#include "nau/project_tools/project_info.h"
#include "nau/project_tools/project_manager.h"

namespace nau
{
    int initProject(const InitProjectArguments* args)
    {
        NAU_RUN_JOB(NauInitProjectJob, "Project successfully initialized at path {}");
    }
    int rebuildProject(const RebuildProjectArguments* args)
    {
        NAU_RUN_JOB(NauRebuildProjectJob, "Project successfully rebuilt at path {}");
    }
    int upgradeProject(const UpgradeProjectArguments* args)
    {
        NAU_RUN_JOB(NauUpgradeProjectJob, "Project successfully upgraded at path {}");
    }
    int saveProject(const SaveProjectArguments* args)
    {
        NAU_RUN_JOB(NauSaveProjectJob, "Project successfully saved at path {}");
    }
    std::shared_ptr<ProjectInfo> loadProject(const std::string& path)
    {
        nlohmann::json config;
        FileSystem fs;

        std::filesystem::path projectPath = std::filesystem::path(path);
        std::filesystem::path lockfilePath = projectPath / ".lockfile";

        if(fs.exist(lockfilePath))
        {
            LOG_WARN("Project is already loaded or previos loading was interrupted!");
            fs.deleteFile(lockfilePath);
        }

        std::filesystem::path configPath = std::filesystem::path(path) / fs.findFirst(projectPath, FileSystemExtensions::g_configExtension);
        LOG_FASSERT(!fs.exist(configPath), "Project config not found!");

        std::stringstream ss;
        fs.readFile(configPath, ss);

        config = nlohmann::json::parse(ss.str());
        auto info = std::make_shared<ProjectInfo>(config.template get<ProjectInfo>());
        LOG_FASSERT(!info, "Failed to load project info");

        fs.createFile(projectPath / ".lockfile");

        return info;
    }
    bool unloadProject(const std::string& path)
    {
        FileSystem fs;
        std::filesystem::path lockfilePath = std::filesystem::path(path) / ".lockfile";
        return fs.deleteFile(lockfilePath);
    }
    NauVersion getVersion()
    {
        return NauVersion(NAU_VERSION);
    }
}  // namespace nau