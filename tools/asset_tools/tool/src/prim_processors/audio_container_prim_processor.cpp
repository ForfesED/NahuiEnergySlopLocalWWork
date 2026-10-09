// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "audio_container_prim_processor.h"
#include "nau/asset_tools/compilers/audio_container_compilers.h"
#include "nau/usd_meta_tools/usd_meta_info.h"

namespace nau
{
    namespace prim_processors
    {
        bool AudioContainerPrimProcessor::canProcess(const nau::UsdMetaInfo& metaInfo) const
        {
            return metaInfo.type == getType();
        }

        nau::Result<AssetMetaInfo> AudioContainerPrimProcessor::process(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            compilers::UsdAudioContainerCompiler compiler;

            return compiler.compile(stage, outputPath, projectRootPath, metaInfo, folderIndex);
        }
    }  // namespace prim_processors
}  // namespace nau
