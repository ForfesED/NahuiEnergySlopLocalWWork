// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <map>
#include <string>
#include <vector>
#include "nau/asset_tools/asset_info.h"
#include "nau/shared/interface/job.h"
#include "pxr/usd/usd/common.h"

namespace nau
{
    class FileSystem;
    class AssetDatabaseManager;
    struct UsdMetaInfo;

    class ASSET_TOOL_API NauImportAssetsJob final : public Job
    {
    public:
        virtual int run(const struct CommonArguments* const) override;

    private:
        nau::Result<AssetMetaInfo> compileAsset(PXR_NS::UsdStageRefPtr stage, const nau::UsdMetaInfo& metaInfo, const std::string& dbPath, const std::string& projectRootPath, int folderIndex);
        nau::Result<std::vector<AssetMetaInfo>> updateAssetInDatabase(PXR_NS::UsdStageRefPtr stage, std::vector<nau::UsdMetaInfo>& metaArray, const std::filesystem::path& dbPath, const std::string& projectRootPath, AssetDatabaseManager& db, FileSystem& fs);
        
        int importAssets(const struct ImportAssetsArguments* args, FileSystem& fs, AssetDatabaseManager& db);
        int importSingleAsset(const struct FileInfo& file, const std::filesystem::path& dbPath, AssetDatabaseManager& db, FileSystem& fs);
        int compileAssets(const ImportAssetsArguments* args, FileSystem& fs, AssetDatabaseManager& db, std::vector<AssetMetaInfo>& assetsList);
        int compileSingleAsset(const FileInfo& file, const std::filesystem::path& dbPath, const std::string& projectRootPath, AssetDatabaseManager& db, FileSystem& fs, std::vector<AssetMetaInfo>& assetsList);
        nau::Result<AssetMetaInfo> updateAsset(PXR_NS::UsdStageRefPtr stage, nau::UsdMetaInfo& meta, const std::filesystem::path& dbPath, const std::string& projectRootPath, AssetDatabaseManager& db, FileSystem& fs);
    };
};  // namespace nau
