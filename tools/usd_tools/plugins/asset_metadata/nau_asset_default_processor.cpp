// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau_asset_default_processor.h"

#include "nau/NauAssetMetadata/nauAssetMaterial.h"
#include "nau/NauAssetMetadata/nauAssetShader.h"
#include "nau/NauAssetMetadata/nauAssetSound.h"
#include "nau/NauAssetMetadata/nauAssetTexture.h"
#include "nau/NauAssetMetadata/nauAssetVideo.h"
#include "nau/NauAssetMetadata/nauAssetAudioContainer.h"
#include "nau/NauAssetMetadata/nauAssetInput.h"
#include "nau/NauAssetMetadata/nauAssetVFX.h"
#include "nau/NauAssetMetadata/NauAssetUI.h"
#include "nau/NauAssetMetadata/NauGltfAssetMeta.h"
#include "nau/NauAssetMetadata/nauPhysicsMaterial.h"
#include "nau/NauAssetMetadata/nauAssetFont.h"
#include <nau/NauAssetMetadata/nauAssetScene.h>

template< typename AssetType, typename TInfoType>
std::shared_ptr<TInfoType> processsAsset(PXR_NS::UsdPrim prim)
{
    AssetType asset(prim);
    if (!asset)
    {
        return nullptr;
    }
    auto info = std::make_shared<TInfoType>();

    PXR_NS::SdfAssetPath path;
    asset.GetPathAttr().Get(&path);
    info->path = path.GetResolvedPath();
    return info;
}

bool NauAssetDefaultProcessor::process(PXR_NS::UsdPrim prim, nau::UsdMetaInfo& dest)
{
    if (auto info = processsAsset<PXR_NS::UsdNauAssetTexture, nau::ExtraInfoTexture>(prim))
    {
        dest.type = "texture";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetVFX, nau::ExtraInfoVFX>(prim))
    {
        dest.type = "vfx";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetInput, nau::ExtraInfoInput>(prim))
    {
        dest.type = "input";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetSound, nau::ExtraInfoSound>(prim))
    {
        dest.type = "sound";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetAudioContainer, nau::ExtraInfoSound>(prim))
    {
        dest.type = "audio-container";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetVideo, nau::ExtraInfoVideo>(prim))
    {
        dest.type = "video";
        dest.extraInfo = info;
    } 
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetUI, nau::ExtraInfoUI>(prim))
    {
        dest.type = "ui";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauPhysicsMaterial, nau::ExtraInfoUI>(prim))
    {
        dest.type = "physics-material";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetFont, nau::ExtraInfoFont>(prim))
    {
        dest.type = "font";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauAssetScene, nau::ExtraInfoScene>(prim))
    {
        dest.type = "scene";
        dest.extraInfo = info;
    }
    else if (auto info = processsAsset<PXR_NS::UsdNauGltfAssetMeta, nau::ExtraInfoGltf>(prim))
    {
        dest.type = "prim-gltf";
        dest.extraInfo = info;
    }

    return !!dest.extraInfo;
}