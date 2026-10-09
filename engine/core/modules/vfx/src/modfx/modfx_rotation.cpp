// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include "modfx_rotation.h"
#include "math/vfx_random.h"

namespace nau::vfx::modfx
{
    void rotation::modfx_rotation_init(int rnd_seed, float& o_angle, const settings::FxRotation& rotation)
    {
        if (std::abs(rotation.start_min - rotation.start_max) < 0.05f)
        {
            o_angle = rotation.start_min;
        }
        else
        {
            o_angle = nau::math::lerp(rotation.start_min, rotation.start_max, vfx::math::dafx_frnd(rnd_seed));
        } 
    }
}  // namespace nau::vfx::modfx
