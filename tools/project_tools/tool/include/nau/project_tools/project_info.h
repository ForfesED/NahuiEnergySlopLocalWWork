// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "nau/shared/version.h"

namespace nau
{
    struct EngineInfo
    {
        std::string Version;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(EngineInfo, Version)
    };

    struct ProjectInfo
    {
        std::string ProjectName;
        std::string Description;
        NauVersion ProjectVersion;
        std::string DefaultScene;
        std::vector<std::string> Dependencies;
        EngineInfo Engine;
    };

    inline void to_json(nlohmann::json& json, const ProjectInfo& s)
    {
        json["ProjectName"] = s.ProjectName;
        json["Description"] = s.Description;
        json["ProjectVersion"] = s.ProjectVersion;
        json["DefaultScene"] = s.DefaultScene;
        json["Dependencies"] = s.Dependencies;
        json["Engine"] = s.Engine;
    }

    inline void from_json(const nlohmann::json& json, ProjectInfo& s)
    {
        json.at("ProjectName").get_to(s.ProjectName);
        json.at("Description").get_to(s.Description);
        s.ProjectVersion = NauVersion(json.at("ProjectVersion").get<std::string>());
        json.at("DefaultScene").get_to(s.DefaultScene);
        json.at("Dependencies").get_to(s.Dependencies);
        json.at("Engine").get_to(s.Engine);
    }
}  // namespace nau