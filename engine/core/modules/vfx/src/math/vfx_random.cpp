// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "vfx_random.h"

namespace nau::vfx
{
    uint math::dafx_fastrnd(rnd_seed_ref seed)
    {
        seed = (214013 * (seed % 0xffffff) + 2531011);
        return (seed >> 16) & DAFX_RND_MAX;
    }

    uint math::dafx_uirnd(rnd_seed_ref seed)
    {
        return dafx_fastrnd(seed);
    }

    float math::dafx_frnd(rnd_seed_ref seed)
    {
        return float(dafx_fastrnd(seed)) * DAFX_RND_MAX_INV;
    }

    float math::dafx_srnd(rnd_seed_ref seed)
    {
        return dafx_frnd(seed) * 2.f - 1.f;
    }

    nau::math::Vector2 math::dafx_srnd_vec2(rnd_seed_ref seed)
    {
        nau::math::Vector2 v;
        v.setX(dafx_srnd(seed));
        v.setY(dafx_srnd(seed));
        return v;
    }

    nau::math::Vector3 math::dafx_srnd_vec3(rnd_seed_ref seed)
    {
        nau::math::Vector3 v;
        v.setX(dafx_srnd(seed));
        v.setY(dafx_srnd(seed));
        v.setZ(dafx_srnd(seed));
        return v;
    }

    nau::math::Vector2 math::dafx_frnd_vec2(rnd_seed_ref seed)
    {
        nau::math::Vector2 v;
        v.setX(dafx_frnd(seed));
        v.setY(dafx_frnd(seed));
        return v;
    }

    nau::math::Vector3 math::dafx_frnd_vec3(rnd_seed_ref seed)
    {
        nau::math::Vector3 v;
        v.setX(dafx_frnd(seed));
        v.setY(dafx_frnd(seed));
        v.setZ(dafx_frnd(seed));
        return v;
    }

    nau::math::Vector4 math::dafx_frnd_vec4(rnd_seed_ref seed)
    {
        nau::math::Vector4 v;
        v.setX(dafx_frnd(seed));
        v.setY(dafx_frnd(seed));
        v.setZ(dafx_frnd(seed));
        v.setW(dafx_frnd(seed));
        return v;
    }

    rnd_seed_t math::dafx_calc_instance_rnd_seed(uint gid, uint dispatch_seed)
    {
        rnd_seed_t base = gid;
        dafx_fastrnd(base);
        return dispatch_seed + base;
    }
}
