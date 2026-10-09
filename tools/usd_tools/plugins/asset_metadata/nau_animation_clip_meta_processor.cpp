// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau_animation_clip_meta_processor.h"

#include "nau/NauAssetMetadata/nauAnimationClipMeta.h"
#include "nau/NauAssetMetadata/nauSkeletalAnimationClipMeta.h"

bool NauAnimationClipMetaProcessor::process(PXR_NS::UsdPrim prim, nau::UsdMetaInfo& dest)
{
    PXR_NS::UsdNauAnimationClipMeta asset(prim);
    if (!asset)
    {
        return false;
    }

    PXR_NS::SdfAssetPath path;
    asset.GetPathAttr().Get(&path);

    auto info = std::make_shared<nau::ExtraInfoAnimation>();
    info->path = path.GetResolvedPath();

    PXR_NS::SdfPathVector targets;
    asset.GetSourceRel().GetTargets(&targets);
    if (!targets.empty())
    {
        info->source = targets[0].GetAsString();
    }

    dest.type = "animation";
    dest.extraInfo = info;
    return true;
}

bool NauSkeletalAnimationClipMetaProcessor::process(PXR_NS::UsdPrim prim, nau::UsdMetaInfo& dest)
{
    PXR_NS::UsdNauSkeletalAnimationClipMeta asset(prim);
    if (!asset)
    {
        return false;
    }

    PXR_NS::SdfAssetPath path;
    asset.GetPathAttr().Get(&path);

    auto info = std::make_shared<nau::ExtraInfoAnimation>();
    info->path = path.GetResolvedPath();

    PXR_NS::SdfPathVector targets;
    asset.GetSourceRel().GetTargets(&targets);
    if (!targets.empty())
    {
        info->source = targets[0].GetAsString();
    }

    dest.type = "prim-animation-skeleton";
    dest.extraInfo = info;
    return true;
}

const nau::MetaArgs& NauAnimationClipMetaGenerator::getDefaultArgs() const
{
    static const nau::MetaArgs defaultArgs =
    {
    };

    return defaultArgs;
}

PXR_NS::UsdPrim NauAnimationClipMetaGenerator::generate(PXR_NS::UsdPrim source, PXR_NS::UsdStagePtr stage, const PXR_NS::SdfPath& dest, const nau::MetaArgs& args)
{
    auto meta = PXR_NS::UsdNauAnimationClipMeta::Define(stage, dest);

    auto sourcePath = source.GetPath().GetAsString();
    sourcePath.replace(sourcePath.find("/Asset"), 6, "/root");
    meta.CreateSourceRel().SetTargets({ pxr::SdfPath { sourcePath }  });

    return meta.GetPrim();
}

const nau::MetaArgs& NauSkeletalAnimationClipMetaGenerator::getDefaultArgs() const
{
    static const nau::MetaArgs defaultArgs =
    {
    };

    return defaultArgs;
}

PXR_NS::UsdPrim NauSkeletalAnimationClipMetaGenerator::generate(PXR_NS::UsdPrim source, PXR_NS::UsdStagePtr stage, const PXR_NS::SdfPath& dest, const nau::MetaArgs& args)
{
    auto meta = PXR_NS::UsdNauSkeletalAnimationClipMeta::Define(stage, dest);

    auto sourcePath = source.GetPath().GetAsString();
    sourcePath.replace(sourcePath.find("/Asset"), 6, "/root");
    meta.CreateSourceRel().SetTargets({ pxr::SdfPath { sourcePath } });

    return meta.GetPrim();
}