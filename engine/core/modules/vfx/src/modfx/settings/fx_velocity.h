// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "nau/dataBlock/dag_dataBlock.h"

namespace nau::vfx::modfx::settings
{
    enum class StartType
    {
        POINT = 0,
        VEC = 1,
        START_SHAPE = 2
    };

    enum class AddType
    {
        POINT = 0,
        VEC = 1,
        CONE = 2
    };

    enum class ForceFieldNoiseType
    {
        VELOCITY_ADD = 0,
        POS_OFFSET = 1
    };

    struct FxInitVelocityPoint
    {
        FxInitVelocityPoint() :
            offset(nau::math::Vector3::zero())
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addPoint3("offset", offset);
        }

        bool load(const nau::DataBlock* blk)
        {
            offset = blk->getPoint3("offset", nau::math::Vector3::zero());
            return true;
        }

        nau::math::Vector3 offset;
    };

    struct FxInitVelocityVec
    {
        FxInitVelocityVec() :
            vec(nau::math::Vector3::zero())
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addPoint3("vec", vec);
        }

        bool load(const nau::DataBlock* blk)
        {
            vec = blk->getPoint3("vec", nau::math::Vector3::zero());
            return true;
        }

        nau::math::Vector3 vec;
    };

    struct FxVelocityStart
    {
        FxVelocityStart() :
            enabled(false),
            vel_min(0.0f),
            vel_max(0.0f),
            vec_rnd(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addReal("vel_min", vel_min);
            blk->addReal("vel_max", vel_max);
            blk->addReal("vec_rnd", vec_rnd);
            blk->addInt("type", static_cast<int>(type));

            auto* pointBlock = blk->addNewBlock("point");
            point.save(pointBlock);

            auto* vecBlock = blk->addNewBlock("vec");
            vec.save(vecBlock);
        }

        bool load(const nau::DataBlock* blk)
        {
            enabled = blk->getBool("enabled", false);
            vel_min = blk->getReal("vel_min", 0.0f);
            vel_max = blk->getReal("vel_max", 0.0f);
            vec_rnd = blk->getReal("vec_rnd", 0.0f);
            type = static_cast<StartType>(blk->getInt("type", static_cast<int>(StartType::POINT)));

            if (auto* pointBlock = blk->getBlockByName("point"))
            {
                if (!point.load(pointBlock))
                    return false;
            }

            if (auto* vecBlock = blk->getBlockByName("vec"))
            {
                if (!vec.load(vecBlock))
                    return false;
            }

            return true;
        }

        bool enabled;
        float vel_min;
        float vel_max;
        float vec_rnd;
        StartType type;
        FxInitVelocityPoint point;
        FxInitVelocityVec vec;
    };

    struct FxInitVelocityCone
    {
        FxInitVelocityCone() :
            vec(nau::math::Vector3::zero()),
            offset(nau::math::Vector3::zero()),
            width_top(0.0f),
            width_bottom(0.0f),
            height(0.0f),
            center_power(0.0f),
            border_power(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addPoint3("vec", vec);
            blk->addPoint3("offset", offset);
            blk->addReal("width_top", width_top);
            blk->addReal("width_bottom", width_bottom);
            blk->addReal("height", height);
            blk->addReal("center_power", center_power);
            blk->addReal("border_power", border_power);
        }

        bool load(const nau::DataBlock* blk)
        {
            vec = blk->getPoint3("vec", nau::math::Vector3::zero());
            offset = blk->getPoint3("offset", nau::math::Vector3::zero());
            width_top = blk->getReal("width_top", 0.0f);
            width_bottom = blk->getReal("width_bottom", 0.0f);
            height = blk->getReal("height", 0.0f);
            center_power = blk->getReal("center_power", 0.0f);
            border_power = blk->getReal("border_power", 0.0f);

            return true;
        }

        nau::math::Vector3 vec;
        nau::math::Vector3 offset;
        float width_top;
        float width_bottom;
        float height;
        float center_power;
        float border_power;
    };

    struct FxVelocityAdd
    {
        FxVelocityAdd() :
            enabled(false),
            apply_emitter_transform(false),
            vel_min(0.0f),
            vel_max(0.0f),
            vec_rnd(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addBool("apply_emitter_transform", apply_emitter_transform);
            blk->addReal("vel_min", vel_min);
            blk->addReal("vel_max", vel_max);
            blk->addReal("vec_rnd", vec_rnd);
            blk->addInt("type", static_cast<int>(type));

            auto* pointBlock = blk->addNewBlock("point");
            point.save(pointBlock);

            auto* vecBlock = blk->addNewBlock("vec");
            vec.save(vecBlock);

            auto* coneBlock = blk->addNewBlock("cone");
            cone.save(coneBlock);
        }

        bool load(const nau::DataBlock* blk)
        {
            enabled = blk->getBool("enabled", false);
            apply_emitter_transform = blk->getBool("apply_emitter_transform", false);
            vel_min = blk->getReal("vel_min", 0.0f);
            vel_max = blk->getReal("vel_max", 0.0f);
            vec_rnd = blk->getReal("vec_rnd", 0.0f);
            type = static_cast<AddType>(blk->getInt("type", static_cast<int>(AddType::POINT)));

            if (auto* pointBlock = blk->getBlockByName("point"))
            {
                if (!point.load(pointBlock))
                    return false;
            }

            if (auto* vecBlock = blk->getBlockByName("vec"))
            {
                if (!vec.load(vecBlock))
                    return false;
            }

            if (auto* coneBlock = blk->getBlockByName("cone"))
            {
                if (!cone.load(coneBlock))
                    return false;
            }

            return true;
        }

        bool enabled;
        bool apply_emitter_transform;
        float vel_min;
        float vel_max;
        float vec_rnd;
        AddType type;
        FxInitVelocityPoint point;
        FxInitVelocityVec vec;
        FxInitVelocityCone cone;
    };

    struct FxForceFieldVortex
    {
        FxForceFieldVortex() :
            enabled(false),
            axis_direction(nau::math::Vector3::zero()),
            direction_rnd(0.0f),
            axis_position(nau::math::Vector3::zero()),
            position_rnd(nau::math::Vector3::zero()),
            rotation_speed_min(0.0f),
            rotation_speed_max(0.0f),
            pull_speed_min(0.0f),
            pull_speed_max(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addPoint3("axis_direction", axis_direction);
            blk->addReal("direction_rnd", direction_rnd);
            blk->addPoint3("axis_position", axis_position);
            blk->addPoint3("position_rnd", position_rnd);
            blk->addReal("rotation_speed_min", rotation_speed_min);
            blk->addReal("rotation_speed_max", rotation_speed_max);
            blk->addReal("pull_speed_min", pull_speed_min);
            blk->addReal("pull_speed_max", pull_speed_max);
        }

        bool load(const nau::DataBlock* blk)
        {
            enabled = blk->getBool("enabled", false);
            axis_direction = blk->getPoint3("axis_direction", nau::math::Vector3::zero());
            direction_rnd = blk->getReal("direction_rnd", 0.0f);
            axis_position = blk->getPoint3("axis_position", nau::math::Vector3::zero());
            position_rnd = blk->getPoint3("position_rnd", nau::math::Vector3::zero());
            rotation_speed_min = blk->getReal("rotation_speed_min", 0.0f);
            rotation_speed_max = blk->getReal("rotation_speed_max", 0.0f);
            pull_speed_min = blk->getReal("pull_speed_min", 0.0f);
            pull_speed_max = blk->getReal("pull_speed_max", 0.0f);

            return true;
        }

        bool enabled;
        nau::math::Vector3 axis_direction;
        float direction_rnd;
        nau::math::Vector3 axis_position;
        nau::math::Vector3 position_rnd;
        float rotation_speed_min;
        float rotation_speed_max;
        float pull_speed_min;
        float pull_speed_max;
    };

    struct FxForceFieldNoise
    {
        FxForceFieldNoise() :
            enabled(false),
            type(ForceFieldNoiseType::VELOCITY_ADD),
            pos_scale(0.0f),
            power_scale(0.0f),
            power_rnd(0.0f),
            power_per_part_rnd(0.0f)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addInt("type", static_cast<int>(type));
            blk->addReal("pos_scale", pos_scale);
            blk->addReal("power_scale", power_scale);
            blk->addReal("power_rnd", power_rnd);
            blk->addReal("power_per_part_rnd", power_per_part_rnd);
        }

        bool load(const nau::DataBlock* blk)
        {
            enabled = blk->getBool("enabled", false);
            type = static_cast<ForceFieldNoiseType>(blk->getInt("type", static_cast<int>(ForceFieldNoiseType::VELOCITY_ADD)));
            pos_scale = blk->getReal("pos_scale", 0.0f);
            power_scale = blk->getReal("power_scale", 0.0f);
            power_rnd = blk->getReal("power_rnd", 0.0f);
            power_per_part_rnd = blk->getReal("power_per_part_rnd", 0.0f);

            return true;
        }

        bool enabled;
        ForceFieldNoiseType type;
        float pos_scale;
        float power_scale;
        float power_rnd;
        float power_per_part_rnd;
    };

    struct FxForceField
    {
        void save(nau::DataBlock* blk) const
        {
            auto* vortexBlock = blk->addNewBlock("vortex");
            vortex.save(vortexBlock);

            auto* noiseBlock = blk->addNewBlock("noise");
            noise.save(noiseBlock);
        }

        bool load(const nau::DataBlock* blk)
        {
            if (auto* vortexBlock = blk->getBlockByName("vortex"))
            {
                if (!vortex.load(vortexBlock))
                    return false;
            }

            if (auto* noiseBlock = blk->getBlockByName("noise"))
            {
                if (!noise.load(noiseBlock))
                    return false;
            }

            return true;
        }

        FxForceFieldVortex vortex;
        FxForceFieldNoise noise;
    };

    struct FxWind
    {
        FxWind() :
            enabled(false),
            directional_force(0.0f),
            directional_freq(0.0f),
            turbulence_force(0.0f),
            turbulence_freq(0.0f),
            impulse_wind(false),
            impulse_wind_force(0.0f)
        {
        }

        // TODO Move to common methods
        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addReal("directional_force", directional_force);
            blk->addReal("directional_freq", directional_freq);
            blk->addReal("turbulence_force", turbulence_force);
            blk->addReal("turbulence_freq", turbulence_freq);
            blk->addBool("impulse_wind", impulse_wind);
            blk->addReal("impulse_wind_force", impulse_wind_force);
        }

        bool load(const nau::DataBlock* blk)
        {
            enabled = blk->getBool("enabled", false);
            directional_force = blk->getReal("directional_force", 0.0f);
            directional_freq = blk->getReal("directional_freq", 0.0f);
            turbulence_force = blk->getReal("turbulence_force", 0.0f);
            turbulence_freq = blk->getReal("turbulence_freq", 0.0f);
            impulse_wind = blk->getBool("impulse_wind", false);
            impulse_wind_force = blk->getReal("impulse_wind_force", 0.0f);

            return true;
        }

        bool enabled;
        float directional_force;
        float directional_freq;
        float turbulence_force;
        float turbulence_freq;
        bool impulse_wind;
        float impulse_wind_force;
    };

    struct FxVelocity
    {
        FxVelocity() :
            enabled(false),
            mass(0.0f),
            drag_coeff(0.0f),
            drag_to_rad_k(0.0f),
            apply_gravity(false),
            gravity_transform(false),
            apply_parent_velocity(false)
        {
        }

        void save(nau::DataBlock* blk) const
        {
            blk->addBool("enabled", enabled);
            blk->addReal("mass", mass);
            blk->addReal("drag_coeff", drag_coeff);
            blk->addReal("drag_to_rad_k", drag_to_rad_k);
            blk->addBool("apply_gravity", apply_gravity);
            blk->addBool("gravity_transform", gravity_transform);
            blk->addBool("apply_parent_velocity", apply_parent_velocity);

            auto* startBlock = blk->addNewBlock("start");
            start.save(startBlock);

            auto* addBlock = blk->addNewBlock("add");
            add.save(addBlock);

            auto* forceFieldBlock = blk->addNewBlock("force_field");
            force_field.save(forceFieldBlock);

            auto* windBlock = blk->addNewBlock("wind");
            wind.save(windBlock);
        }

        bool load(const nau::DataBlock* blk)
        {
            if (!blk)
            {
                return false;
            }

            enabled = blk->getBool("enabled", false);
            mass = blk->getReal("mass", 0.0f);
            drag_coeff = blk->getReal("drag_coeff", 0.0f);
            drag_to_rad_k = blk->getReal("drag_to_rad_k", 0.0f);
            apply_gravity = blk->getBool("apply_gravity", false);
            gravity_transform = blk->getBool("gravity_transform", false);
            apply_parent_velocity = blk->getBool("apply_parent_velocity", false);

            if (auto* startBlock = blk->getBlockByName("start"))
            {
                if (!start.load(startBlock))
                    return false;
            }

            if (auto* addBlock = blk->getBlockByName("add"))
            {
                if (!add.load(addBlock))
                    return false;
            }

            if (auto* forceFieldBlock = blk->getBlockByName("force_field"))
            {
                if (!force_field.load(forceFieldBlock))
                    return false;
            }

            if (auto* windBlock = blk->getBlockByName("wind"))
            {
                if (!wind.load(windBlock))
                    return false;
            }

            return true;
        }

        bool enabled;

        float mass;
        float drag_coeff;
        float drag_to_rad_k;

        bool apply_gravity;
        bool gravity_transform;
        bool apply_parent_velocity;

        FxVelocityStart start;
        FxVelocityAdd add;
        FxForceField force_field;
        FxWind wind;
    };
}  // namespace nau::vfx::modfx::settings
