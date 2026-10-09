// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "animation_prim_processor.h"

#include "nau/asset_tools/compilers/animation_compilers.h"
#include "nau/usd_meta_tools/usd_meta_info.h"

namespace nau
{
    namespace prim_processors
    {
        bool AnimationPrimProcessor::canProcess(const nau::UsdMetaInfo& metaInfo) const
        {
            return metaInfo.type == getType();
        }

        nau::Result<AssetMetaInfo> AnimationPrimProcessor::process(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPaht, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            compilers::UsdKeyFrameAnimationCompiler compiler;

            return compiler.compile(stage, outputPath, projectRootPaht, metaInfo, folderIndex);
        }

        bool SkeletalAnimationPrimProcessor::canProcess(const nau::UsdMetaInfo& metaInfo) const
        {
            return metaInfo.type == getType();
        }

        nau::Result<AssetMetaInfo> SkeletalAnimationPrimProcessor::process(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPaht, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            compilers::UsdSkeletalAnimationCompiler compiler;

            return compiler.compile(stage, outputPath, projectRootPaht, metaInfo, folderIndex);
        }

        bool GltfPrimProcessor::canProcess(const nau::UsdMetaInfo& metaInfo) const
        {
            return metaInfo.type == getType();
        }

        nau::Result<AssetMetaInfo> GltfPrimProcessor::process(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath,const std::string& projectRootPaht, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            compilers::GltfSkeletalAnimationCompiler compiler;

            return compiler.compile(stage, outputPath, projectRootPaht, metaInfo, folderIndex);
        }
    }  // namespace prim_processors
}  // namespace nau
