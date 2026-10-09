// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>

#include "nau/asset_tools/asset_info.h"
#include "nau/asset_tools/interface/asset_compiler.h"

namespace nau::compilers
{
    class UsdAudioContainerCompiler final : public IAssetCompiler
    {
    public:
        std::string_view ext() const override
        {
            return ".nsound";
        }
        bool canCompile(const std::string& path) const override
        {
            return true;
        }
        nau::Result<AssetMetaInfo> compile(
            PXR_NS::UsdStageRefPtr stage,
            const std::string& outputPath,
            const std::string& projectRootPath,
            const nau::UsdMetaInfo& metaInfo,
            int folderIndex) override;
    };

}  // namespace nau::compilers
