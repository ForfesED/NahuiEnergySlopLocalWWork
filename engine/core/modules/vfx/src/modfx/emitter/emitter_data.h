// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "emitter_type.h"

namespace nau::vfx::modfx
{
    class EmitterData
    {
    public:
        EmitterType type = EmitterType::LINEAR;

        struct FixedData
        {
            int count = 0;
        };

        struct BurstData
        {
            int countMin = 0;
            int countMax = 0;
            int cycles = 0;
            float period = 0.0f;
            float lifeLimit = 0.0f;
            int elemLimit = 0;
        };

        struct LinearData
        {
            int countMin = 0;
            int countMax = 0;
            float lifeLimit = 0.0f;
            bool instant = false;
        };

        float delay = 0.0f;
        float globalLifeLimitMin = 0.0f;
        float globalLifeLimitMax = 0.0f;

        float minEmissionFactor = 0.0f;

        FixedData fixedData;
        BurstData burstData;
        LinearData linearData;
    };
}  // namespace nau::vfx::modfx