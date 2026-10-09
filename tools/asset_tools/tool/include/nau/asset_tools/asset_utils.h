// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <filesystem>
#include <string>
namespace nau
{
    struct AssetMetaInfo;
    class FileSystem;
    struct UsdMetaInfo;

    namespace utils
    {
        int getAssetSubDir(const std::filesystem::path& path, FileSystem& fs);
        void* getUsdPlugin(const std::string& pluginName);

        namespace compilers
        {
            std::filesystem::path ensureOutputPath(const std::string& outputPath, const AssetMetaInfo& metaInfo, std::string ext);
            int copyAsset(const std::string& path, const std::string& outputPath, const nau::UsdMetaInfo& metaInfo, int folderIndex, std::string ext);
            int copyFileToExportDirectory(const std::string& from, const std::string& projectRoot);
        }
    }  // namespace utils
}  // namespace nau
