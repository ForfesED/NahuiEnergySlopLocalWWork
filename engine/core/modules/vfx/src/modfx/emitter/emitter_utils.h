// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "emitter_data.h"
#include "emitter_state.h"

namespace nau::vfx::modfx::emitter_utils
{
    void create_emitter_state(EmitterState& state, const EmitterData& data, int elem_limit, float emission_factor);
    int update_emitter(EmitterState& state, float dt);
}  // namespace nau::vfx::modfx::emitter_utils
