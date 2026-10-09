// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>
#include <vector>
#include <map>
#include "nau/project_tools/project_api.h"

namespace nau
{
    class PROJECT_TOOL_API StringProcessor
    {
    public:
        int countMatches(const std::string& content, const std::string& pattern);
        void toLower(std::string& string);
        void trim(std::string& string);
        void replace(std::string& string, char from, char to);
        bool split(const std::string& string, const std::string& delimiter, std::vector<std::string>& out);
        std::string modifyExpandedValue(const std::string& content, const std::vector<std::string>& modifiers);
        std::string processRegexMatches(const std::string& pattern, const std::string& content, std::map<std::string, std::string>& projectProperties, int& count);
    };
}