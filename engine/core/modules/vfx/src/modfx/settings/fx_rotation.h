// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "nau/dataBlock/dag_dataBlock.h"

namespace nau::vfx::modfx::settings
{
    struct FxDynamicRotation
    {
        FxDynamicRotation() :
            enabled(false),
            vel_min(0.0f),
            vel_max(0.0f)
        {
        }

        // TODO Move to common methods
        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addReal("vel_min", vel_min);
            blk->addReal("vel_max", vel_max);
        }

        bool load(const nau::DataBlock* blk)
        {
            if (!blk)
                return false;

            enabled = blk->getBool("enabled", false);
            vel_min = blk->getReal("vel_min", 0.0f);
            vel_max = blk->getReal("vel_max", 0.0f);

            return true;
        }

        bool enabled;
        
        float vel_min;
        float vel_max;
    };

    struct FxRotation
    {
        FxRotation() :
            enabled(false),
            start_min(0.0f),
            start_max(0.0f),
            dynamic(FxDynamicRotation())
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addReal("start_min", start_min);
            blk->addReal("start_max", start_max);

            auto* dynamicBlock = blk->addNewBlock("dynamic");
            dynamic.save(dynamicBlock);
        }

        bool load(const nau::DataBlock* blk)
        {
            if (!blk)
            {
                return false;
            }

            enabled = blk->getBool("enabled", false);
            start_min = blk->getReal("start_min", 0.0f);
            start_max = blk->getReal("start_max", 0.0f);

            if (auto* dynamicBlock = blk->getBlockByName("dynamic"))
            {
                if (!dynamic.load(dynamicBlock))
                    return false;
            }

            return true;
        }

        bool enabled;

        float start_min;
        float start_max;

        FxDynamicRotation dynamic;
    };
}  // namespace nau::vfx::modfx::settings