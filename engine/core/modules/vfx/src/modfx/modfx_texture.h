// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "settings/fx_texture.h"

namespace nau::vfx::modfx::texture
{
    void modfx_texture_sim(float life_k, int& frameId, const settings::FxTexture& texture);
}