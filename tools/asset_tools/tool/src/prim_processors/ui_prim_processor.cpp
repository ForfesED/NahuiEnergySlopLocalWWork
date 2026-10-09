// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "ui_prim_processor.h"
#include <nau/asset_tools/asset_compiler.h>
#include "nau/usd_meta_tools/usd_meta_info.h"

namespace nau
{
    namespace prim_processors
    {
        bool UIPrimProcessor::canProcess(const nau::UsdMetaInfo& metaInfo) const
        {
            return metaInfo.type == getType();
        }

        nau::Result<AssetMetaInfo> UIPrimProcessor::process(
            PXR_NS::UsdStageRefPtr stage,
            const std::string& outputPath,
            const std::string& projectRootPath,
            const nau::UsdMetaInfo& metaInfo,
            int folderIndex)
        {
            return callCompilerWithoutSource("ui", stage, outputPath, projectRootPath, metaInfo, folderIndex);
        }
    }  // namespace prim_processors
}  // namespace nau
