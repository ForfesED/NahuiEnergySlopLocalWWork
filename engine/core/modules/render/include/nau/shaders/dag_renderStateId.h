// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/3d/dag_renderStateId.h"
#include "nau/util/dag_generationRefId.h"

namespace shaders
{
    namespace render_states
    {
        NAU_RENDER_EXPORT RenderStateId create(const RenderState &state);
        NAU_RENDER_EXPORT void set(RenderStateId);
        NAU_RENDER_EXPORT const RenderState &get(RenderStateId id);
        NAU_RENDER_EXPORT uint32_t get_count();
    } // namespace render_states
} // namespace shaders
