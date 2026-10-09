// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "modfx/settings/fx_life.h"
#include "modfx/settings/fx_velocity.h"

#include "modfx/modfx_ren_data.h"
#include "modfx/modfx_sim_data.h"

#include "nau/math/dag_color.h"
#include "nau/math/math.h"
#include "nau/dataBlock/dag_dataBlock.h"

namespace nau::vfx
{
    class NAU_ABSTRACT_TYPE IVFXInstance : public virtual IRefCounted
    {
        NAU_INTERFACE(nau::vfx::IVFXInstance, IRefCounted)

        virtual void serialize(nau::DataBlock* blk) const = 0;
        virtual bool deserialize(const nau::DataBlock* blk) = 0;

        virtual void setTransform(const nau::math::Matrix4& transform) = 0;
        virtual nau::math::Matrix4 transform() const = 0;
    
        virtual void update(float dt) = 0;
        virtual void render(const nau::math::Matrix4& view, const nau::math::Matrix4& projection) = 0;
    };
}  // namespace nau::vfx