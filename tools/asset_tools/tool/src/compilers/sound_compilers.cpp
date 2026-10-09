// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/asset_tools/compilers/sound_compilers.h"

#include "nau/asset_tools/asset_compiler.h"
#include "nau/asset_tools/asset_info.h"
#include "nau/asset_tools/asset_utils.h"
#include "nau/assets/asset_container.h"
#include "nau/assets/asset_container_builder.h"
#include "nau/assets/asset_manager.h"
#include "nau/assets/asset_ref.h"
#include "nau/assets/texture_asset_accessor.h"
#include "nau/diag/logging.h"
#include "nau/io/file_system.h"
#include "nau/io/io_constants.h"
#include "nau/io/memory_stream.h"
#include "nau/io/stream.h"
#include "nau/io/stream_utils.h"
#include "nau/io/virtual_file_system.h"
#include "nau/service/service.h"
#include "nau/service/service_provider.h"
#include "nau/shared/file_system.h"
#include "nau/shared/logger.h"

namespace nau
{
    namespace compilers
    {
        nau::Result<AssetMetaInfo> copyAndPrepare(const std::string& ext,
                                                  const std::string& outputPath,
                                                  const std::string& projectRootPath,
                                                  const nau::UsdMetaInfo& metaInfo,
                                                  int folderIndex)
        {
            auto extraInfo = reinterpret_cast<ExtraInfoSound*>(metaInfo.extraInfo.get());

            // copy all sounds to the folder, where AudioContainer can reach it
            if (nau::utils::compilers::copyAsset(extraInfo->path, outputPath, metaInfo, folderIndex, ext) != 0)
            {
                return NauMakeError("Failed to copy {} to {}!", extraInfo->path, outputPath);
            }

            //if (nau::utils::compilers::copyFileToExportDirectory(extraInfo->path, projectRootPath) != 0)
            //{
            //    return NauMakeError("Failed to copy {} to export directory!", extraInfo->path);
            //}

            return makeAssetMetaInfo(extraInfo->path, metaInfo.uid, std::format("{}/{}{}", folderIndex, toString(metaInfo.uid), ext), ext.substr(1), "RawAudio");
        }

        nau::Result<AssetMetaInfo> Mp3AssetCompiler::compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            return copyAndPrepare(ext().data(), outputPath, projectRootPath, metaInfo, folderIndex);
        }

        nau::Result<AssetMetaInfo> WavAssetCompiler::compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            return copyAndPrepare(ext().data(), outputPath, projectRootPath, metaInfo, folderIndex);
        }

        nau::Result<AssetMetaInfo> FlacAssetCompiler::compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            return copyAndPrepare(ext().data(), outputPath, projectRootPath, metaInfo, folderIndex);
        }
    }  // namespace compilers
}  // namespace nau
