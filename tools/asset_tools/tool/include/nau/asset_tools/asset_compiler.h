// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <cassert>
#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "nau/asset_tools/interface/asset_compiler.h"
#include "nau/shared/macro.h"
#include "nau/usd_meta_tools/usd_meta_info.h"

namespace nau
{
    struct AssetMetaInfo;

    namespace compilers
    {
        class CopyAssetCompiler final : public IAssetCompiler
        {
        public:
            std::string_view ext() const override
            {
                return "*";
            }
            bool canCompile(const std::string& path) const override
            {
                return true;
            }
            nau::Result<AssetMetaInfo> compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndexo) override;
        };
    }  // namespace compilers

    enum class CompilerResult
    {
        Success = 0,
        CompilerNotFound,
        InternalError,
        CompilationProhibited,
        Max
    };

    bool isExtensionSupported(const std::string& ext);

    nau::Result<AssetMetaInfo> callCompiler(const std::string& sourceFilePath, PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex);
    nau::Result<AssetMetaInfo> callCompilerWithoutSource(const std::string& compilerName, PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex);
    std::string getTargetExtension(const std::string& ext);
    std::shared_ptr<compilers::IAssetCompiler> getAssetCompiler(const std::string& ext);
};  // namespace nau
