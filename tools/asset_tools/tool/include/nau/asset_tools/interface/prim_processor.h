// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>

#include "nau/asset_tools/asset_info.h"
#include "pxr/usd/usd/common.h"

namespace nau
{
    struct UsdMetaInfo;

    namespace prim_processors
    {
        class ASSET_TOOL_API IPrimProcessor
        {
        public:
            virtual std::string_view getType() const = 0;
            virtual bool canProcess(const nau::UsdMetaInfo& metaInfo) const = 0;
            virtual nau::Result<AssetMetaInfo> process(
                PXR_NS::UsdStageRefPtr stage,
                const std::string& outputPath,
                const std::string& projectRootPath,
                const nau::UsdMetaInfo& metaInfo,
                int folderIndex
            ) = 0;
        };
    }  // namespace compilers
}  // namespace nau
