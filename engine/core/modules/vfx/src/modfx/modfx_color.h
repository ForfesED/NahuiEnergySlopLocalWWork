// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "settings/fx_color.h"

namespace nau::vfx::modfx::color
{
    void modfx_color_init(int rnd_seed, nau::math::Color4& o_color, const settings::FxColor& color);
    void modfx_color_sim(int rnd_seed, float life_k, nau::math::Color4& o_color, const settings::FxColor& color);
}  // namespace nau::vfx::modfx::color