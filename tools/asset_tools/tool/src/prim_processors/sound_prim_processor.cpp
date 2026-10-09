// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "sound_prim_processor.h"

#include <nau/asset_tools/asset_compiler.h>
#include <nau/shared/file_system.h>
#include <nau/shared/logger.h>

#include <fstream>

#include "nau/asset_tools/asset_utils.h"
#include "nau/math/math.h"
#include "nau/serialization/json_utils.h"
#include "nau/usd_meta_tools/usd_meta_manager.h"
#include "usd_translator/usd_mesh_adapter.h"
#include "usd_translator/usd_mesh_composer.h"
#include "usd_translator/usd_translator.h"

namespace nau
{
    namespace prim_processors
    {
        bool SoundPrimProcessor::canProcess(const nau::UsdMetaInfo& metaInfo) const
        {
            return metaInfo.type == getType();
        }

        nau::Result<AssetMetaInfo> SoundPrimProcessor::process(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            auto extraData = reinterpret_cast<ExtraInfoSound*>(metaInfo.extraInfo.get());
            return callCompiler(extraData->path, stage, outputPath, projectRootPath, metaInfo, folderIndex);
        }
    }  // namespace prim_processors
}  // namespace nau
