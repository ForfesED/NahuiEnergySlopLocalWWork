// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/physics/jolt/jolt_physics_shapes.h"
#include "nau/physics/jolt/jolt_physics_math.h"
#include "nau/physics/jolt/jolt_debug_renderer.h"

namespace nau::physics::jolt
{
    JoltSphereShape::JoltSphereShape(float radius, const JPH::PhysicsMaterial* material)
        : JPH::SphereShape(radius, material)
    {
    }

    void JoltSphereShape::Draw(JPH::DebugRenderer* renderer, JPH::RMat44Arg centerOfMassTransform,
        JPH::Vec3Arg scale, JPH::ColorArg color, bool /*useMaterialColors*/, bool /*drawWireframe*/) const
    {
        renderer->DrawSphere(centerOfMassTransform, GetRadius() * scale.Abs().GetX(), color);
    }
}
