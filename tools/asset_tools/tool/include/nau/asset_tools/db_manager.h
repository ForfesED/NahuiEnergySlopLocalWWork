// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <map>

#include "nau/asset_tools/asset_info.h"
#include "nau/shared/file_system.h"
#include "nlohmann/json.hpp"

namespace nau
{
    class Logger;
    struct Uid;

    struct ASSET_TOOL_API AssetCache
    {
        std::vector<AssetMetaInfo> content;
        NAU_CLASS_FIELDS(
            CLASS_FIELD(content))
    };

    class ASSET_TOOL_API AssetDatabaseManager
    {
    public:
        AssetDatabaseManager() = default;
        ~AssetDatabaseManager() = default;
        AssetDatabaseManager(const AssetDatabaseManager&) = delete;
        AssetDatabaseManager(AssetDatabaseManager&&) = delete;
        AssetDatabaseManager& operator=(const AssetDatabaseManager&) = delete;

        static AssetDatabaseManager& instance();

        bool load(const std::string_view& cachePath);
        bool isLoaded() const;
        bool save();
        bool addOrReplace(const AssetMetaInfo& metaInfo);
        int update(std::vector<AssetMetaInfo>& list);
        bool exist(const Uid& uid);
        bool compiled(const Uid& uid);
        bool compiled(const std::string_view& sourcePath);
        nau::Result<Uid> findIf(const std::string_view& sourcePath);
        nau::Result<int> getDbFolderIndex(const Uid& uid);

        inline size_t size() const
        {
            return m_cache.content.size();
        }

        inline std::vector<AssetMetaInfo>& assets()
        {
            return m_cache.content;
        }

        nau::Result<AssetMetaInfo> get(const Uid& uid);
    private:
        AssetCache m_cache;
        FileSystem m_fs;
        std::string m_cachePath;
        std::filesystem::path m_dbFile;
        bool m_isLoaded = false;
    };
};  // namespace nau