// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once

#include "settings/fx_life.h"

namespace nau::vfx::modfx::life
{
    void modfx_life_init(int rnd_seed, float& life_norm, const settings::FxLife& life);
    void modfx_life_sim(int rnd_seed, float life_limit_rcp, float dt, const settings::FxLife& life, float& o_life_norm);
}

