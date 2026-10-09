// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "nau/dataBlock/dag_dataBlock.h"

namespace nau::vfx::modfx::settings
{
    struct FxRadius
    {
        FxRadius() :
            enabled(false),
            rad_min(0.0f),
            rad_max(0.0f)
        {
        }

        // TODO Move to common methods
        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addReal("rad_min", rad_min);
            blk->addReal("rad_max", rad_max);
        }

        bool load(const nau::DataBlock* blk)
        {
            if (!blk)
            {
                return false;
            }

            enabled = blk->getBool("enabled", false);
            rad_min = blk->getReal("rad_min", 0.0f);
            rad_max = blk->getReal("rad_max", 0.0f);

            return true;
        }

        bool enabled;

        float rad_min;
        float rad_max;
    };
}  // namespace nau::vfx::modfx::settings