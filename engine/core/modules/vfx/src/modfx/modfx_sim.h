// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "modfx_ren_data.h"
#include "modfx_sim_data.h"

#include "settings/fx_life.h"
#include "settings/fx_radius.h"
#include "settings/fx_velocity.h"
#include "settings/fx_color.h"
#include "settings/fx_texture.h"

namespace nau::vfx::modfx::sim
{
    void modfx_apply_sim(modfx::ModfxRenData& rdata, modfx::ModfxSimData& sdata, float dt, const settings::FxLife& life, const settings::FxRadius& radius, const settings::FxVelocity& velocity, const settings::FxColor& color, const settings::FxTexture& texture);
}

