// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/rtti/rtti_object.h"
#include "nau/math/math.h"
#include "graphics_assets/material_asset.h"

namespace nau::vfx
{
    class IVFXInstance;

    struct NAU_VFX_EXPORT NAU_ABSTRACT_TYPE VFXManager
    {
        NAU_TYPEID(nau::vfx::VFXManager)

        virtual std::shared_ptr<IVFXInstance> addInstance(const nau::MaterialAssetView::Ptr material) = 0;
        virtual void removeInstance(std::shared_ptr<IVFXInstance> instance) = 0;

        virtual void update(float dt) = 0;
        virtual void render(const nau::math::Matrix4& view, const nau::math::Matrix4& projection) = 0;
    };
}  // namespace nau::vfx