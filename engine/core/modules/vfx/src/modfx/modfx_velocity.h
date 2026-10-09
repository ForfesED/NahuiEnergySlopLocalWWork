// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/math.h"
#include "settings/fx_velocity.h"

namespace nau::vfx::modfx::velocity
{
    void modfx_velocity_add(int rnd_seed, nau::math::Vector3 pos, nau::math::Vector3& o_velocity, const settings::FxVelocity& velocity);
    void modfx_velocity_force_field_vortex(float life_k, int rnd_seed, nau::math::Vector3 pos, nau::math::Vector3& o_velocity, const settings::FxVelocity& velocity);
    void modfx_velocity_force_resolver(float dt, float mass, float drag_c, float friction_k, nau::math::Vector3 grav_vec, nau::math::Vector3& o_pos, nau::math::Vector3& o_vel);

    void modfx_velocity_init(nau::math::Vector3& pos, nau::math::Vector3& pos_v, nau::math::Vector3& o_velocity, int rnd_seed, const settings::FxVelocity& velocity);
    void modfx_velocity_sim(int rnd_seed, float life_k, float dt, float radius, nau::math::Vector3& o_pos, nau::math::Vector3& o_ofs_pos, nau::math::Vector3& o_velocity, const settings::FxVelocity& velocity);
}
