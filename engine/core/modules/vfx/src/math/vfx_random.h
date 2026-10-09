// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once

#include "nau/math/math.h"

#define DAFX_RND_MAX (0x7fff)
#define DAFX_RND_MAX_INV (1.f / DAFX_RND_MAX)

#define uint int
#define rnd_seed_t uint
#define rnd_seed_ref rnd_seed_t&

namespace nau::vfx::math
{
    uint dafx_fastrnd(rnd_seed_ref seed);
    uint dafx_uirnd(rnd_seed_ref seed);
    float dafx_frnd(rnd_seed_ref seed);
    float dafx_srnd(rnd_seed_ref seed);

    nau::math::Vector2 dafx_srnd_vec2(rnd_seed_ref seed);
    nau::math::Vector3 dafx_srnd_vec3(rnd_seed_ref seed);
    nau::math::Vector2 dafx_frnd_vec2(rnd_seed_ref seed);
    nau::math::Vector3 dafx_frnd_vec3(rnd_seed_ref seed);
    nau::math::Vector4 dafx_frnd_vec4(rnd_seed_ref seed);

    rnd_seed_t dafx_calc_instance_rnd_seed(uint gid, uint dispatch_seed);
}  // namespace nau::vfx::math
