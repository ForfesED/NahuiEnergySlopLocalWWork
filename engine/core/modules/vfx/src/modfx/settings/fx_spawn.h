// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "nau/dataBlock/dag_dataBlock.h"

#include "../emitter/emitter_type.h"

namespace nau::vfx::modfx::settings
{
    struct FxSpawnLinear
    {
        FxSpawnLinear() :
            count_min(0),
            count_max(0)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addInt("count_min", count_min);
            blk->addInt("count_max", count_max);
        }

        bool load(const nau::DataBlock* blk)
        {
            count_min = blk->getInt("count_min", 0);
            count_max = blk->getInt("count_max", 0);

            return true;
        }

        int count_min;
        int count_max;
    };

    struct FxSpawnBurst
    {
        FxSpawnBurst() :
            count_min(0),
            count_max(0),
            cycles(0),
            period(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addInt("count_min", count_min);
            blk->addInt("count_max", count_max);

            blk->addInt("cycles", cycles);

            blk->addReal("period", period);
        }

        bool load(const nau::DataBlock* blk)
        {
            count_min = blk->getInt("count_min", 0);
            count_max = blk->getInt("count_max", 0);

            cycles = blk->getInt("cycles", 0);

            period = blk->getReal("period", 0.0f);

            return true;
        }

        int count_min;
        int count_max;

        int cycles;

        float period;
    };

    struct FxSpawnFixed
    {
        FxSpawnFixed() :
            count(0)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addInt("count", count);
        }

        bool load(const nau::DataBlock* blk)
        {
            count = blk->getInt("count", 0);

            return true;
        }

        int count;
    };

    struct FxSpawn
    {
        FxSpawn() :
            type(EmitterType::LINEAR),
            linear(FxSpawnLinear()),
            burst(FxSpawnBurst()),
            fixed(FxSpawnFixed())
        {
        }

        // TODO Move to common methods
        void save(nau::DataBlock* blk) const
        {
            blk->addInt("type", static_cast<int>(type));

            auto* linearBlock = blk->addNewBlock("linear");
            linear.save(linearBlock);

            auto* burstBlock = blk->addNewBlock("burst");
            burst.save(burstBlock);

            auto* fixedBlock = blk->addNewBlock("fixed");
            fixed.save(fixedBlock);
        }

        bool load(const nau::DataBlock* blk)
        {
            type = static_cast<EmitterType>(blk->getInt("type", static_cast<int>(EmitterType::LINEAR)));  // ��������� ��� ��������

            if (auto* linearBlock = blk->getBlockByName("linear"))
            {
                if (!linear.load(linearBlock))
                    return false;
            }

            if (auto* burstBlock = blk->getBlockByName("burst"))
            {
                if (!burst.load(burstBlock))
                    return false;
            }

            if (auto* fixedBlock = blk->getBlockByName("fixed"))
            {
                if (!fixed.load(fixedBlock))
                    return false;
            }

            return true;
        }

        EmitterType type;
        FxSpawnLinear linear;
        FxSpawnBurst burst;
        FxSpawnFixed fixed;
    };
}  // namespace nau::vfx::modfx::settings