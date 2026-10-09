// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_physics_rigid_body_mesh.h"
#include "nau/NauPhysicsSchema/rigidBodyMesh.h"
#include "nau/math/math.h"

namespace UsdTranslator
{
    namespace
    {
        constexpr std::string_view g_typeName = "RigidBodyMesh";
    }

    PhysicsRigidMeshAdapter::PhysicsRigidMeshAdapter(PXR_NS::UsdPrim prim)
        : PhysicsRigidBodyAdapter(prim)
    {}

    std::string_view PhysicsRigidMeshAdapter::getType() const
    {
        return g_typeName;
    }

    void PhysicsRigidMeshAdapter::fillRigidBodyComponent(nau::physics::RigidBodyComponent& component) const
    {
        PXR_NS::PhysicsRigidBodyMesh meshBody{getPrim()};
        PXR_NS::SdfAssetPath sdfPath;
        meshBody.GetModelMeshAttr().Get(&sdfPath);

        component.setMeshCollision(getMeshAsset(sdfPath));
        component.setUseConvexHullForCollision(false);
    }

    DEFINE_TRANSLATOR(PhysicsRigidMeshAdapter, "RigidBodyMesh"_tftoken);
}