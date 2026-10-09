// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "settings/fx_radius.h"

namespace nau::vfx::modfx::radius
{
    void modfx_radius_init(int rnd_seed, float& o_rad, const settings::FxRadius& radius);
    void modfx_radius_sim(int rnd_seed, float life_k, float& o_rad, const settings::FxRadius& radius);
}