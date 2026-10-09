// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <iostream>

#include "nau/shared/api.h"
#include "nlohmann/json.hpp"

namespace nau
{
    namespace util
    {
        std::string SHARED_API unescape(const std::string& input);
        std::string SHARED_API getIdeExtension(const std::string& presetName);
        void SHARED_API mergeJsonRecursive(nlohmann::json& target, const nlohmann::json& source);

        bool SHARED_API validateEnvironment();
        bool isVisualStudioInstalled();
        bool SHARED_API checkEnvironmentVariables();
    }  // namespace util
}  // namespace nau