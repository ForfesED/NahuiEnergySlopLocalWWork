// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "nau/math/dag_color.h"

namespace nau::vfx::modfx
{
    struct ModfxRenData
    {
        ModfxRenData() 
            : pos(nau::math::Vector3::zero())
            , radius(0.5f)
            , angle(0.0f)
            , color(nau::math::Color4(1.0f, 0.0f, 0.0f, 1.0f))
            , frame_idx(0)
            , frame_flags(0)
            , frame_blend(0)
            , life_norm(0)
            , emission_fade(1)
            , up_vec(nau::math::Vector3::zero())
            , right_vec(nau::math::Vector3::zero())
            , pos_offset(nau::math::Vector3::zero())
            , unique_id(0)
        {}

        void clear()
        {
            pos = nau::math::Vector3::zero();
            pos_offset = nau::math::Vector3::zero();
            radius = 0.0f;
            color = nau::math::Color4(1.0f, 1.0f, 1.0f, 1.0f);
            angle = 0.0f;

            frame_idx = 0;
            frame_flags = 0;
            frame_blend = 0.0f;

            life_norm = 0.0f;
            emission_fade = 0.0f;

            up_vec = nau::math::Vector3::zero();
            right_vec = nau::math::Vector3::zero();
            velocity_length = 0.0f;

            unique_id = 0;
        }

        nau::math::Vector3 pos;
        nau::math::Vector3 pos_offset;
        float radius;
        nau::math::Color4 color;
        float angle;

        int frame_idx;
        int frame_flags;
        float frame_blend;

        float life_norm;
        float emission_fade;

        nau::math::Vector3 up_vec;
        nau::math::Vector3 right_vec;
        float velocity_length;

        int unique_id;
    };
}

