// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/asset_tools/asset_info.h"
#include "pxr/usd/usd/common.h"

namespace nau
{
    struct UsdMetaInfo;
    nau::Result<AssetMetaInfo> processMeta(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const UsdMetaInfo& metaInfo, int folderIndex);
}  // namespace nau
