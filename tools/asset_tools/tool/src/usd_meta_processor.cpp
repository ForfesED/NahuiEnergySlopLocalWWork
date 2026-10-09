// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/asset_tools/usd_meta_processor.h"

#include <nau/asset_tools/db_manager.h>

#include "nau/asset_tools/interface/prim_processor.h"
#include "nau/usd_meta_tools/usd_meta_info.h"
#include "prim_processors/animation_prim_processor.h"
#include "prim_processors/font_prim_processor.h"
#include "prim_processors/material_prim_processor.h"
#include "prim_processors/mesh_prim_processor.h"
#include "prim_processors/scene_prim_processor.h"
#include "prim_processors/shader_prim_processor.h"
#include "prim_processors/texture_prim_processor.h"
#include "prim_processors/ui_prim_processor.h"
#include "prim_processors/vfx_prim_processor.h"
#include "prim_processors/input_prim_processor.h"
#include "prim_processors/audio_container_prim_processor.h"
#include "prim_processors/sound_prim_processor.h"
#include "prim_processors/physics_prim_processor.h"

namespace nau
{
    static std::map<std::string, std::shared_ptr<prim_processors::IPrimProcessor>> primProcessors = {
        { "animation",               std::make_shared<prim_processors::AnimationPrimProcessor>() },
        { "prim-animation-skeleton", std::make_shared<prim_processors::SkeletalAnimationPrimProcessor>() },
        { "prim-gltf",               std::make_shared<prim_processors::GltfPrimProcessor>() },
        { "mesh",                    std::make_shared<prim_processors::MeshPrimProcessor>() },
        { "texture",                 std::make_shared<prim_processors::TexturePrimProcessor>() },
        { "material",                std::make_shared<prim_processors::MaterialPrimProcessor>() },
        { "shader",                  std::make_shared<prim_processors::ShaderPrimProcessor>() },
        { "scene",                   std::make_shared<prim_processors::ScenePrimProcessor>()},
        { "font",                    std::make_shared<prim_processors::FontPrimProcessor>()},
        { "ui",                      std::make_shared<prim_processors::UIPrimProcessor>()},
        { "vfx",                     std::make_shared<prim_processors::VFXPrimProcessor>()},
        { "input",                   std::make_shared<prim_processors::InputPrimProcessor>()},
        { "audio-container",         std::make_shared<prim_processors::AudioContainerPrimProcessor>()},
        { "sound",                   std::make_shared<prim_processors::SoundPrimProcessor>()},
        { "physics-material",        std::make_shared<prim_processors::PhysicsPrimProcessor>() }
    };

    nau::Result<AssetMetaInfo> processMeta(PXR_NS::UsdStageRefPtr stage, const std::string& outputPath, const std::string& projectRootPath, const UsdMetaInfo& metaInfo, int folderIndex)
    {
        FileSystem fs;

        AssetDatabaseManager& dbManager = AssetDatabaseManager::instance();

        NAU_VERIFY(dbManager.isLoaded(), "Asset database not loaded!");

        auto processor = primProcessors.find(metaInfo.type);

        if (processor == primProcessors.end())
        {
            return NauMakeError("Expected valid prim, got something else {}", metaInfo.type.c_str());
        }

        if (!processor->second->canProcess(metaInfo))
        {
            return NauMakeError("This prim cannot be processed {}", metaInfo.type.c_str());
        }

        return processor->second->process(stage, outputPath, projectRootPath, metaInfo, folderIndex);
    }
}  // namespace nau
