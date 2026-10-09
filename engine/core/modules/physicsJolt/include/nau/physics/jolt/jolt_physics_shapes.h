// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

namespace nau::physics::jolt
{
    class JoltSphereShape : public JPH::SphereShape
    {
    public:
        JoltSphereShape(float radius, const JPH::PhysicsMaterial* material = nullptr);

        void Draw(JPH::DebugRenderer* renderer, JPH::RMat44Arg centerOfMassTransform, JPH::Vec3Arg scale,
            JPH::ColorArg color, bool inUseMaterialColors, bool drawWireframe) const override;
    };
}
