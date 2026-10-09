// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "nau/dataBlock/dag_dataBlock.h"

namespace nau::vfx::modfx::settings
{
    enum PositionType
    {
        SPHERE,
        CYLINDER,
        CONE,
        BOX
    };

    struct FxInitPositionSphere
    {
        FxInitPositionSphere() :
            volume(0.0f),
            radius(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addReal("volume", volume);
            blk->addReal("radius", radius);
        }

        bool load(const nau::DataBlock* blk)
        {
            volume = blk->getReal("volume", 0.0f);
            radius = blk->getReal("radius", 0.0f);
            return true;
        }

        float volume;
        float radius;
    };

    struct FxInitPositionCylinder
    {
        FxInitPositionCylinder() :
            vec(nau::math::Vector3::zero()),
            volume(0.0f),
            radius(0.0f),
            height(0.0f),
            random_burst(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addPoint3("vec", vec);
            blk->addReal("volume", volume);
            blk->addReal("radius", radius);
            blk->addReal("height", height);
            blk->addReal("random_burst", random_burst);
        }

        bool load(const nau::DataBlock* blk)
        {
            vec = blk->getPoint3("vec", nau::math::Vector3::zero());
            volume = blk->getReal("volume", 0.0f);
            radius = blk->getReal("radius", 0.0f);
            height = blk->getReal("height", 0.0f);
            random_burst = blk->getReal("random_burst", 0.0f);
            return true;
        }

        nau::math::Vector3 vec;
        float volume;
        float radius;
        float height;
        float random_burst;
    };

    struct FxInitPositionCone
    {
        FxInitPositionCone() :
            vec(nau::math::Vector3::zero()),
            volume(0.0f),
            width_top(0.0f),
            width_bottom(0.0f),
            height(0.0f),
            random_burst(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addPoint3("vec", vec);
            blk->addReal("volume", volume);
            blk->addReal("width_top", width_top);
            blk->addReal("width_bottom", width_bottom);
            blk->addReal("height", height);
            blk->addReal("random_burst", random_burst);
        }

        bool load(const nau::DataBlock* blk)
        {
            vec = blk->getPoint3("vec", nau::math::Vector3::zero());
            volume = blk->getReal("volume", 0.0f);
            width_top = blk->getReal("width_top", 0.0f);
            width_bottom = blk->getReal("width_bottom", 0.0f);
            height = blk->getReal("height", 0.0f);
            random_burst = blk->getReal("random_burst", 0.0f);
            return true;
        }

        nau::math::Vector3 vec;
        float volume;
        float width_top;
        float width_bottom;
        float height;
        float random_burst;
    };

    struct FxInitPositionBox
    {
        FxInitPositionBox() :
            volume(0.0f),
            width(0.0f),
            height(0.0f),
            depth(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addReal("volume", volume);
            blk->addReal("width", width);
            blk->addReal("height", height);
            blk->addReal("depth", depth);
        }

        bool load(const nau::DataBlock* blk)
        {
            volume = blk->getReal("volume", 0.0f);
            width = blk->getReal("width", 0.0f);
            height = blk->getReal("height", 0.0f);
            depth = blk->getReal("depth", 0.0f);
            return true;
        }

        float volume;
        float width;
        float height;
        float depth;
    };

    struct FxPosition
    {
        FxPosition() :
            enabled(false),
            type(PositionType::BOX),
            volume(0.0f),
            offset(nau::math::Vector3::zero())
        {
        }

        // TODO Move to common methods
        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addInt("type", static_cast<int>(type));
            blk->addReal("volume", volume);
            blk->addPoint3("offset", offset);

            auto* sphereBlock = blk->addNewBlock("sphere");
            sphere.save(sphereBlock);

            auto* cylinderBlock = blk->addNewBlock("cylinder");
            cylinder.save(cylinderBlock);

            auto* coneBlock = blk->addNewBlock("cone");
            cone.save(coneBlock);

            auto* boxBlock = blk->addNewBlock("box");
            box.save(boxBlock);
        }

        bool load(const nau::DataBlock* blk)
        {
            if (!blk)
            {
                return false;
            }

            enabled = blk->getBool("enabled", false);
            type = static_cast<PositionType>(blk->getInt("type", static_cast<int>(PositionType::BOX)));
            volume = blk->getReal("volume", 0.0f);
            offset = blk->getPoint3("offset", nau::math::Vector3::zero());

            if (auto* sphereBlock = blk->getBlockByName("sphere"))
            {
                if (!sphere.load(sphereBlock))
                    return false;
            }

            if (auto* cylinderBlock = blk->getBlockByName("cylinder"))
            {
                if (!cylinder.load(cylinderBlock))
                    return false;
            }

            if (auto* coneBlock = blk->getBlockByName("cone"))
            {
                if (!cone.load(coneBlock))
                    return false;
            }

            if (auto* boxBlock = blk->getBlockByName("box"))
            {
                if (!box.load(boxBlock))
                    return false;
            }

            return true;
        }

        bool enabled;
        PositionType type;
        float volume;
        nau::math::Vector3 offset;

        FxInitPositionSphere sphere;
        FxInitPositionCylinder cylinder;
        FxInitPositionCone cone;
        FxInitPositionBox box;
    };
}  // namespace nau::vfx::modfx::settings
