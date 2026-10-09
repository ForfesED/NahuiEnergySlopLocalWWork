// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>

#include "nau/asset_tools/asset_info.h"
#include "nau/asset_tools/interface/asset_compiler.h"
#include "nau/usd_meta_tools/usd_meta_manager.h"

namespace nau
{
    namespace compilers
    {
        // Being called from UsdAssetCompiler TODO: custom extension
        class UsdMaterialAssetCompiler final : public IAssetCompiler
        {
        public:
            std::string_view ext() const override;
            bool canCompile(const std::string& path) const override;
            nau::Result<AssetMetaInfo> compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& info, int folderIndex) override;
        private:
            RuntimeValue::Ptr resolveValue(PXR_NS::VtValue value);
            std::string resolveShaderPath(const std::string& path);
        };
    }  // namespace compilers
}  // namespace nau
