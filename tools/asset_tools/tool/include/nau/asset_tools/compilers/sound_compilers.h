// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>

#include "nau/asset_tools/asset_info.h"
#include "nau/asset_tools/interface/asset_compiler.h"

namespace nau
{
    namespace compilers
    {
        class Mp3AssetCompiler final : public IAssetCompiler
        {
        public:
            std::string_view ext() const override
            {
                return ".mp3";
            }
            bool canCompile(const std::string& path) const override
            {
                return true;
            }
            nau::Result<AssetMetaInfo> compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex) override;
        };

        class WavAssetCompiler final : public IAssetCompiler
        {
        public:
            std::string_view ext() const override
            {
                return ".wav";
            }
            bool canCompile(const std::string& path) const override
            {
                return true;
            }
            nau::Result<AssetMetaInfo> compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex) override;
        };

        class FlacAssetCompiler final : public IAssetCompiler
        {
        public:
            std::string_view ext() const override
            {
                return ".flac";
            }
            bool canCompile(const std::string& path) const override
            {
                return true;
            }
            nau::Result<AssetMetaInfo> compile(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex) override;
        };
    }  // namespace compilers
}  // namespace nau
