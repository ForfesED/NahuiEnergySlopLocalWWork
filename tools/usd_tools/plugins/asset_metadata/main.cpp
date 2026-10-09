// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/NauAssetMetadata/nauAnimationClipMeta.h"
#include "nau/NauAssetMetadata/NauAssetUI.h"
#include "nau/NauAssetMetadata/nauAssetAudioContainer.h"
#include "nau/NauAssetMetadata/nauAssetMaterial.h"
#include "nau/NauAssetMetadata/nauAssetShader.h"
#include "nau/NauAssetMetadata/nauAssetSound.h"
#include "nau/NauAssetMetadata/nauAssetTexture.h"
#include "nau/NauAssetMetadata/nauAssetVFX.h"
#include "nau/NauAssetMetadata/nauAssetScene.h"
#include "nau/NauAssetMetadata/nauAssetInput.h"
#include "nau/NauAssetMetadata/nauAssetVideo.h"
#include "nau/NauAssetMetadata/nauGltfAssetMeta.h"
#include "nau/NauAssetMetadata/tokens.h"
#include "nau/NauAssetMetadata/nauPhysicsMaterial.h"
#include "nau/usd_meta_tools/usd_meta_manager.h"
#include "nau/usd_meta_tools/usd_meta_generator.h"
#include "nau/NauAssetMetadata/nauAssetFont.h"
#include "nau_asset_geom_processor.h"
#include "nau_asset_default_processor.h"
#include "nau_asset_material_processor.h"
#include "nau_animation_clip_meta_processor.h"

DefineNauMetaPlugin
{
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetMesh, NauAssetMeshProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetGroup, NauAssetGroupProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetMaterial, NauAssetMaterialProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetShader, NauAssetShaderProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAnimationClipMeta, NauAnimationClipMetaProcessor);
    DefineNauMetaProcessor(PXR_NS::TfToken("AnimationClip"), NauAnimationClipMetaProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauSkeletalAnimationClipMeta, NauSkeletalAnimationClipMetaProcessor);

    DeclarePrimMetaGenerator(NauAssetGeomGenerator, "Xform");
    DeclarePrimMetaGenerator(NauAssetGeomGenerator, "Mesh");

    DeclareMetaTemplate(NauAssetMaterialGenerator, "Material");
    DeclareMetaGenerator(NauAssetShaderGenerator, { ".hlsl" });

    // not specialized
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetTexture, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetSound, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetAudioContainer, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetVideo, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetVFX, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetUI, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetInput, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauPhysicsMaterial, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::TfToken("PhysicsMaterial"), NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetFont, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauAssetScene, NauAssetDefaultProcessor);
    DefineNauMetaProcessor(PXR_NS::UsdTokens->NauGltfAssetMeta, NauAssetDefaultProcessor);

    DeclarePrimMetaGenerator(NauAssetGeomGenerator, "Xform");
    DeclarePrimMetaGenerator(NauAssetGeomGenerator, "Mesh");
    DeclarePrimMetaGenerator(NauAnimationClipMetaGenerator, "AnimationClip");
    DeclarePrimMetaGenerator(NauSkeletalAnimationClipMetaGenerator, "SkelRoot");

    DeclarePrimMetaGenerator(NauAssetDefaultPrimGenerator<PXR_NS::UsdNauAssetAudioContainer>, "AudioContainer");
    DeclarePrimMetaGenerator(NauAssetDefaultPrimGenerator<PXR_NS::UsdNauAssetVFX>, "VFXInstance");
    DeclarePrimMetaGenerator(NauAssetDefaultPrimGenerator<PXR_NS::UsdNauPhysicsMaterial>, "PhysicsMaterial");
    DeclarePrimMetaGenerator(NauAssetDefaultPrimGenerator<PXR_NS::UsdNauAssetUI>, "UI");
    DeclarePrimMetaGenerator(NauAssetDefaultPrimGenerator<PXR_NS::UsdNauAssetInput>, "InputAction");
    DeclareMetaGenerator(NauAssetDefaultGenerator<PXR_NS::UsdNauAssetTexture>, { ".jpg", ".png", ".dds", ".bmp", "tiff"});
    DeclareMetaGenerator(NauAssetDefaultGenerator<PXR_NS::UsdNauAssetSound>, { ".wav", ".mp3", ".flac" });
    DeclareMetaGenerator(NauAssetDefaultGenerator<PXR_NS::UsdNauAssetVideo>, { ".avi", ".mp4", ".mov" });
    DeclareMetaGenerator(NauAssetDefaultGenerator<PXR_NS::UsdNauAssetFont>, {".fnt", ".bmfc"});
    DeclareMetaGenerator(NauAssetDefaultGenerator<PXR_NS::UsdNauAssetScene>, {".nausd_scene"});
    DeclareMetaGenerator(NauAssetDefaultGenerator<PXR_NS::UsdNauGltfAssetMeta>, { ".ngltf" });
}
