// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "nau/math/dag_color.h"

namespace nau::vfx::modfx
{
    struct ModfxSimData
    {
        ModfxSimData() 
            : life_norm(0.0f)
            , flags(0)
            , velocity(nau::math::Vector3::zero())
            , em_color(nau::math::Color4(1.0f, 1.0f, 1.0f, 1.0f))
        {}

        void clear()
        {
            life_norm = 0.0f;

            rnd_seed = 0;
            flags = 0;

            velocity = nau::math::Vector3::zero();
            em_color = nau::math::Color4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        float life_norm;

        int rnd_seed;
        int flags;

        nau::math::Vector3 velocity;
        nau::math::Color4 em_color;
    };
}
