// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/usd_meta_tools/usd_meta_manager.h"
#include "nau/usd_meta_tools/usd_meta_generator.h"

class NauAssetMaterialProcessor final : public nau::IMetaProcessor
{
    bool process(PXR_NS::UsdPrim prim, nau::UsdMetaInfo& dest) override;
};

class NauAssetMaterialGenerator final : public nau::IMetaGeneratorTemplate
{
    bool generate(PXR_NS::UsdStagePtr stage, const nau::MetaArgs& args) override;
    const nau::MetaArgs& getDefaultArgs() const override;
};


class NauAssetShaderProcessor final : public nau::IMetaProcessor
{
    bool process(PXR_NS::UsdPrim prim, nau::UsdMetaInfo& dest) override;
};

class NauAssetShaderGenerator final : public nau::IMetaGenerator
{
    const nau::MetaArgs& getDefaultArgs() const override;
    bool generate(const std::filesystem::path& path, PXR_NS::UsdStagePtr stage, const nau::MetaArgs& args) override;
};