// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/rtti/rtti_impl.h"
#include "nau/physics/physics_collision_shapes_factory.h"
#include "nau/physics/jolt/jolt_physics_world.h"

namespace nau::physics::jolt
{
    class JoltPhysicsCollisionShapesFactory final : public ICollisionShapesFactory
    {
        NAU_CLASS(nau::physics::jolt::JoltPhysicsCollisionShapesFactory, rtti::RCPolicy::Concurrent, ICollisionShapesFactory)

    private:
        ICollisionShape::Ptr createGenericCollisionShape(const rtti::TypeInfo& colliderType,
            const RuntimeValue::Ptr& constructionData, nau::physics::IPhysicsMaterial* material = nullptr) const override;

        IConvexHullCollision::Ptr createConvexHullCollision(const IConvexHullCollision::ConstructionData& constructionData,
            nau::physics::IPhysicsMaterial* material = nullptr) const override;

        IMeshCollision::Ptr createMeshCollision(const IMeshCollision::ConstructionData& constructionData) const override;

        ICollisionShape::Ptr createMeshCollisionFromAsset(nau::Ptr<physics::TriMeshAssetView> asset) const override;

        ICollisionShape::Ptr createConvexHullCollisionFromAsset(nau::Ptr<physics::ConvexHullAssetView> asset) const override;
    };
} // namespace nau::physics::jolt

