// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"

namespace nau::vfx::modfx
{
    class EmitterState
    {
    public:
        bool isDistanceBased = false;
        int emissionLimit = 0;

        int batchSize = 0;
        int cyclesCount = 0;
        int cyclesCountRef = 0;
        float lifeLimit = 0.0f;
        float globalLifeLimit = 0.0f;

        float spawnTick = 0.0f;
        float spawnTickRef = 0.0f;
        float shrinkTick = 0.0f;
        float shrinkTickRef = 0.0f;
        float globalLifeLimitRef = 0.0f;

        float tickLimit = 0.0f;

        float totalTickRate = 0.0f;
        float localTickRate = 0.0f;

        float garbageTick = 0.0f;

        float generation = 0.0f;
        float dist = 0.0f;
        float distSq = 0.0f;
        nau::math::Vector3 lastEmittedPos = nau::math::Vector3::zero();
        bool lastEmittedPosValid = false;
    };
}  // namespace nau::vfx::modfx
