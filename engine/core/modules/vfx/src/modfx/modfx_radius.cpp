// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include "modfx_radius.h"
#include "math/vfx_random.h"

namespace nau::vfx::modfx
{
    void radius::modfx_radius_init(int rnd_seed, float& o_rad, const settings::FxRadius& radius)
    {
        if ((radius.rad_min / radius.rad_max) >= 0.95f)
        {
            o_rad = radius.rad_min;
        }
        else
        {
            o_rad = nau::math::lerp(radius.rad_min, radius.rad_max, vfx::math::dafx_frnd(rnd_seed));
        }
    }

    void radius::modfx_radius_sim(int rnd_seed, float life_k, float& o_rad, const settings::FxRadius& radius)
    {
        // TODO Add curves handling
    }
}
