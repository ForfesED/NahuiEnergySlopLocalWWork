// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/asset_tools/compilers/font_compilers.h"

#include <nau/shared/logger.h>
#include <filesystem>

#include "nau/asset_tools/asset_compiler.h"
#include "nau/asset_tools/asset_info.h"
#include "nau/asset_tools/asset_utils.h"

namespace nau
{
    namespace compilers
    {
        bool FontAssetCompiler::canCompile(const std::string& path) const
        {
            return true;
        }

        nau::Result<AssetMetaInfo> FontAssetCompiler::compile(
            PXR_NS::UsdStageRefPtr stage,
            const std::string& outputPath,
            const std::string& projectRootPath,
            const nau::UsdMetaInfo& metaInfo,
            int folderIndex)
        {
            auto extraInfo = reinterpret_cast<ExtraInfoFont*>(metaInfo.extraInfo.get());

            if (nau::utils::compilers::copyAsset(extraInfo->path, outputPath, metaInfo, folderIndex, ".fnt") != 0)
            {
                return NauMakeError("Failed to copy {} to {}!", extraInfo->path, outputPath);
            }

            if (utils::compilers::copyFileToExportDirectory(extraInfo->path, projectRootPath) != 0)
            {
                return NauMakeError("Failed to copy {} to export directory!", extraInfo->path);
            }

            return makeAssetMetaInfo(extraInfo->path, metaInfo.uid, std::format("{}/{}{}", folderIndex, toString(metaInfo.uid), ext().data()), /*Extension of source file*/ "fnt", "Font");
        }
    }  // namespace compilers
}  // namespace nau
