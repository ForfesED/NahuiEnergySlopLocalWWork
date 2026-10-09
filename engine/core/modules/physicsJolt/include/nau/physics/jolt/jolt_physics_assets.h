// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

#include "nau/assets/asset_view.h"
#include "nau/assets/mesh_asset_accessor.h"
#include "nau/physics/physics_assets.h"
#include "nau/rtti/rtti_impl.h"


namespace nau::physics::jolt
{
    /**
     */
    class JoltConvexHullAssetView final : public physics::ConvexHullAssetView
    {
        NAU_CLASS_(nau::physics::jolt::JoltConvexHullAssetView, physics::ConvexHullAssetView)
    public:
        JoltConvexHullAssetView(IMeshAssetAccessor& meshAccessor);

        JPH::ConvexHullShapeSettings& getShapeSettings();

    private:
        std::optional<JPH::ConvexHullShapeSettings> m_convexHullShapeSettings;
    };

    /**
     */
    class JoltTriMeshAssetView final : public physics::TriMeshAssetView
    {
        NAU_CLASS_(nau::physics::jolt::JoltTriMeshAssetView, physics::TriMeshAssetView)
    public:
        JoltTriMeshAssetView(IMeshAssetAccessor& meshAccessor);

        JPH::MeshShapeSettings& getShapeSettings();

    private:
        std::optional<JPH::MeshShapeSettings> m_meshShapeSettings;
    };
}  // namespace nau::physics