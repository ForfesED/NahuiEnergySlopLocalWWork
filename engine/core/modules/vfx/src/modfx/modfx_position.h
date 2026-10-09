// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "settings/fx_position.h"

namespace nau::vfx::modfx::position
{
    float modfx_position_radius_rnd(float rad, float volume, float rnd);

    void modfx_position_init_sphere(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionSphere& sphere);
    void modfx_position_init_box(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionBox& box);
    void modfx_position_init_cone(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionCone& cone);
    void modfx_position_init_cylinder(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionCylinder& cylinder);
    
    void modfx_position_init(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxPosition& positionSettings);
}
