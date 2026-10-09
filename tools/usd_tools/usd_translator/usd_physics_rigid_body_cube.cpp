// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_physics_rigid_body_cube.h"
#include <nau/service/service_provider.h>
#include "nau/physics/physics_world.h"

#include <nau/service/service_provider.h>

#include "nau/NauPhysicsSchema/rigidBodyCube.h"
#include "nau/physics/physics_collision_shapes_factory.h"
#include <pxr/usd/usd/stage.h>

namespace UsdTranslator
{
    namespace
    {
        constexpr std::string_view g_typeName = "RigidBodyCube";
    }

    PhysicsRigidCubeAdapter::PhysicsRigidCubeAdapter(PXR_NS::UsdPrim prim) :
        PhysicsRigidBodyAdapter(prim)
    {
    }

    std::string_view PhysicsRigidCubeAdapter::getType() const
    {
        return g_typeName;
    }

    void PhysicsRigidCubeAdapter::fillRigidBodyComponent(nau::physics::RigidBodyComponent& component) const
    {
        PXR_NS::PhysicsRigidBodyCube cubeBody{getPrim()};

        PXR_NS::GfVec3d extent;
        cubeBody.GetExtentAttr().Get(&extent);

        auto& physShapesFactory = nau::getServiceProvider().get<nau::physics::ICollisionShapesFactory>();
        const nau::math::vec3 colliderExtent {static_cast<float>(extent[0]), static_cast<float>(extent[1]), static_cast<float>(extent[2])};

        // TODO: setup physics material (createMaterial(getPrim()).get())
        [[maybe_unused]] auto& collider = component.getCollisions().addBox(colliderExtent);
    };

    DEFINE_TRANSLATOR(PhysicsRigidCubeAdapter, "RigidBodyCube"_tftoken);
}  // namespace UsdTranslator
