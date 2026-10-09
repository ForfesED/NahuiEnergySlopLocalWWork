// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "mesh_prim_processor.h"

#include <nau/asset_tools/compilers/usd_compilers.h>
#include <nau/shared/file_system.h>
#include <nau/shared/logger.h>

#include "nau/asset_tools/asset_compiler.h"
#include "nau/usd_meta_tools/usd_meta_manager.h"
#include "usd_translator/usd_mesh_adapter.h"
#include "usd_translator/usd_mesh_composer.h"
#include "usd_translator/usd_translator.h"

namespace nau
{
    namespace prim_processors
    {
        bool nau::prim_processors::MeshPrimProcessor::canProcess(const nau::UsdMetaInfo& metaInfo) const
        {
            return metaInfo.type == getType();
        }

        nau::Result<AssetMetaInfo> MeshPrimProcessor::process(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const nau::UsdMetaInfo& metaInfo, int folderIndex)
        {
            compilers::UsdMeshAssetCompiler compiler;

            return compiler.compile(stage, outputPath, projectRootPath, metaInfo, folderIndex);
        }
    }  // namespace prim_processors
}  // namespace nau
