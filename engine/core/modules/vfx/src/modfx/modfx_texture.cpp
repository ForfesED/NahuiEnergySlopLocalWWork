// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include "modfx_texture.h"

namespace nau::vfx::modfx
{
    void texture::modfx_texture_sim(float life_k, int& frameId, const settings::FxTexture& texture)
    {
        frameId = static_cast<int>(life_k * ((texture.frames_x * texture.frames_y) - 1));
    }
}  // namespace nau::vfx::modfx
