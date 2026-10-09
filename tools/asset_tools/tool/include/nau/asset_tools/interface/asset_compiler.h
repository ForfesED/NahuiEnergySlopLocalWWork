// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>
#include "pxr/usd/usd/common.h"
#include "nau/asset_tools/asset_info.h"

namespace nau
{
    struct UsdMetaInfo;

    namespace compilers
    {
        class ASSET_TOOL_API IAssetCompiler
        {
        public:
            virtual std::string_view ext() const = 0;
            virtual bool canCompile(const std::string& path) const = 0;
            virtual nau::Result<AssetMetaInfo> compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex) = 0;
        };
    }  // namespace compilers
}  // namespace nau
