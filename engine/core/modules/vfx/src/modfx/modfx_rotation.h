// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "settings/fx_rotation.h"

namespace nau::vfx::modfx::rotation
{
    void modfx_rotation_init(int rnd_seed, float& o_angle, const settings::FxRotation& rotation);
}