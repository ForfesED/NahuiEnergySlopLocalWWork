// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include "modfx_color.h"
#include "math/vfx_random.h"

namespace nau::vfx::modfx
{
    void color::modfx_color_init(int rnd_seed, nau::math::Color4& o_color, const settings::FxColor& color)
    {
        if (color.start_color == color.end_color)
        {
            o_color = color.start_color;
        }
        else
        {
            const float t = vfx::math::dafx_frnd(rnd_seed);
            o_color = nau::math::Color4(nau::math::lerp(color.start_color.r, color.end_color.r, t),
                                        nau::math::lerp(color.start_color.g, color.end_color.g, t),
                                        nau::math::lerp(color.start_color.b, color.end_color.b, t),
                                        nau::math::lerp(color.start_color.a, color.end_color.a, t));
        }
    }

    void color::modfx_color_sim(int rnd_seed, float life_k, nau::math::Color4& o_color, const settings::FxColor& color)
    {
        if (color.gradient.enabled)
        {
            o_color = color.gradient.gradient.getColorAt(life_k);
        }
    }

}  // namespace nau::vfx::modfx
