// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/assets/asset_view.h"

namespace nau::physics
{
    /**
     */
    struct NAU_ABSTRACT_TYPE TriMeshAssetView : IAssetView
    {
        NAU_INTERFACE(nau::physics::TriMeshAssetView, IAssetView)
    };

    /**
     */
    struct NAU_ABSTRACT_TYPE ConvexHullAssetView : IAssetView
    {
        NAU_INTERFACE(nau::physics::ConvexHullAssetView, IAssetView)
    };

}  // namespace nau::physics
