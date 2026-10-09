// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include "modfx_position.h"
#include "math/vfx_random.h"


namespace nau::vfx::modfx
{
    float position::modfx_position_radius_rnd(float rad, float volume, float rnd)
    {
        float v = 1.0f - (rnd * rnd);
        return nau::math::lerp(volume, 1.0f, v) * rad;
    }

    void position::modfx_position_init_sphere(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionSphere& sphere)
    {
        velocity = normalize(vfx::math::dafx_srnd_vec3(rnd_seed));
        position = velocity * modfx_position_radius_rnd(sphere.radius, sphere.volume, vfx::math::dafx_frnd(rnd_seed));
    }

    void position::modfx_position_init_box(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionBox& box)
    {
        velocity = vfx::math::dafx_srnd_vec3(rnd_seed);
        position = mulPerElem(velocity, nau::math::Vector3(box.width, box.height, box.width));
    }

    void position::modfx_position_init_cone(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionCone& cone)
    {
        rnd_seed_t burst_seed = vfx::math::dafx_fastrnd(dispatch_seed);
        nau::math::Vector3 rnd_sector = vfx::math::dafx_srnd_vec3(burst_seed);
        nau::math::Vector3 yaxis = normalize(lerp(cone.random_burst, cone.vec, rnd_sector));
    
        nau::math::Vector3 origin = -yaxis * cone.height;

        nau::math::Vector3 xaxis = fabsf(yaxis.getY()) > 0.9f ? nau::math::Vector3(1, 0, 0) : nau::math::Vector3(0, 1, 0);
        nau::math::Vector3 zaxis = normalize(cross(xaxis, yaxis));
        xaxis = cross(zaxis, yaxis);

        nau::math::Vector2 rnd = vfx::math::dafx_srnd_vec2(rnd_seed);
        nau::math::Vector3 xo = xaxis * rnd.getX();
        nau::math::Vector3 zo = zaxis * rnd.getY();

        nau::math::Vector3 p = normalize(xo + zo) * modfx_position_radius_rnd(cone.height, cone.volume, vfx::math::dafx_frnd(rnd_seed));
        velocity = normalize(p - origin);
        position = origin + velocity * (cone.height + vfx::math::dafx_frnd(rnd_seed) * cone.width_top) / dot(velocity, yaxis);

        if (cone.width_top < 0)
            velocity *= -1.0f;
    }

    void position::modfx_position_init_cylinder(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxInitPositionCylinder& cylinder)
    {
        rnd_seed_t burst_seed = vfx::math::dafx_fastrnd(dispatch_seed);
        nau::math::Vector3 rnd_sector = vfx::math::dafx_srnd_vec3(burst_seed);
        nau::math::Vector3 v = normalize(nau::math::lerp(cylinder.vec, rnd_sector, cylinder.random_burst));

        nau::math::Vector3 yaxis = fabsf(v.getY()) < 0.9f ? nau::math::Vector3(0, 1, 0) : nau::math::Vector3(1, 0, 0);
        nau::math::Vector3 zaxis = normalize(cross(yaxis, v));
        nau::math::Vector3 xaxis = normalize(cross(v, zaxis));

        velocity = normalize(vfx::math::dafx_srnd(rnd_seed) * xaxis + vfx::math::dafx_srnd(rnd_seed) * zaxis);
        position = velocity * modfx_position_radius_rnd(cylinder.radius, cylinder.volume, vfx::math::dafx_frnd(rnd_seed)) + v * vfx::math::dafx_frnd(rnd_seed) * cylinder.height;
    }

    void position::modfx_position_init(int rnd_seed, int dispatch_seed, nau::math::Vector3& position, nau::math::Vector3& velocity, const settings::FxPosition& positionSettings)
    {
        switch (positionSettings.type)
        {
        case settings::PositionType::SPHERE:
            modfx_position_init_sphere(rnd_seed, dispatch_seed, position, velocity, positionSettings.sphere);
            break;
        case settings::PositionType::CYLINDER:
            modfx_position_init_cylinder(rnd_seed, dispatch_seed, position, velocity, positionSettings.cylinder);
            break;
        case settings::PositionType::CONE:
            modfx_position_init_cone(rnd_seed, dispatch_seed, position, velocity, positionSettings.cone);
            break;
        case settings::PositionType::BOX:
            modfx_position_init_box(rnd_seed, dispatch_seed, position, velocity, positionSettings.box);
            break;
        }
    }
}
