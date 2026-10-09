// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once
#include "nau/dataBlock/dag_dataBlock.h"

namespace nau::vfx::modfx::settings
{
    struct FxLife
    {
        FxLife() :
            part_life_min(0),
            part_life_max(0),
            part_life_rnd_offset(0),
            inst_life_delay(0)
        {
        }

        // TODO Move to common methods
        void save(nau::DataBlock* blk) const
        {
            blk->addReal("part_life_min", part_life_min);
            blk->addReal("part_life_max", part_life_max);
            blk->addReal("part_life_rnd_offset", part_life_rnd_offset);
            blk->addReal("inst_life_delay", inst_life_delay);
        }

        bool load(const nau::DataBlock* blk)
        {
            if (!blk)
            {
                return false;
            }

            part_life_min = blk->getReal("part_life_min", 0.0f);
            part_life_max = blk->getReal("part_life_max", 0.0f);
            part_life_rnd_offset = blk->getReal("part_life_rnd_offset", 0.0f);
            inst_life_delay = blk->getReal("inst_life_delay", 0.0f);

            return true;
        }

        float part_life_min;
        float part_life_max;
        float part_life_rnd_offset;
        float inst_life_delay;
    };
}  // namespace nau::vfx::modfx::settings
