// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_physics_rigid_body_convex_hull.h"

#include <nau/service/service_provider.h>
#include "nau/NauPhysicsSchema/rigidBodyConvexHull.h"
#include "nau/math/math.h"

namespace UsdTranslator
{
    namespace
    {
        constexpr std::string_view g_typeName = "RigidBodyConvexHull";
    }

    PhysicsRigidConvexHullAdapter::PhysicsRigidConvexHullAdapter(PXR_NS::UsdPrim prim) :
        PhysicsRigidBodyAdapter(prim)
    {
    }

    std::string_view PhysicsRigidConvexHullAdapter::getType() const
    {
        return g_typeName;
    }

    void PhysicsRigidConvexHullAdapter::fillRigidBodyComponent(nau::physics::RigidBodyComponent& component) const
    {
        PXR_NS::PhysicsRigidBodyConvexHull hullBody{getPrim()};
        PXR_NS::SdfAssetPath sdfPath;
        hullBody.GetModelMeshAttr().Get(&sdfPath);

        component.setMeshCollision(getMeshAsset(sdfPath));
        component.setUseConvexHullForCollision(true);
    };

    DEFINE_TRANSLATOR(PhysicsRigidConvexHullAdapter, "RigidBodyConvexHull"_tftoken);
}  // namespace UsdTranslator
