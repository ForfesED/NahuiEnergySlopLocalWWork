// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <map>

#include "nau/shared/version.h"
#include "nau/project_tools/project_api.h"

namespace nau
{
    using NauProjectParams = std::map<std::string, std::string>;

    struct NauFileInfo
    {
        std::string path;
        std::string name;
        std::string pureName;
        std::string extension;
        std::string subpath;

        bool updateContent = false;
        bool rename = false;
        bool lowercase = false;
    };

    class PROJECT_TOOL_API NauProjectTemplate
    {
        NauProjectTemplate(const NauProjectTemplate&) = delete;
        NauProjectTemplate &operator=(const NauProjectTemplate&) = delete;
        NauProjectTemplate(NauProjectTemplate&&) = delete;
        NauProjectTemplate &operator=(NauProjectTemplate&&) = delete;

    public:
        explicit NauProjectTemplate(const std::string& templateName, const std::filesystem::path& templatePath, const std::map<std::string, std::string>& projectParams, class FileSystem* fs) noexcept;

        ~NauProjectTemplate() = default;

        inline bool isValid() const { return m_isValid; }
        inline const NauVersion& getVersion() const { return m_version; }
        inline const std::vector<NauFileInfo>& getFiles() const { return m_files; }
        inline const std::filesystem::path& getTemplatePath() const { return m_templatePath; }

    private:
        std::vector<NauFileInfo> m_files;
        std::string m_templateName;
        std::filesystem::path m_templatePath;
        NauVersion m_version;
        bool m_isValid = false;
    };
}