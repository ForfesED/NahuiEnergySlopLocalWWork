// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_accessor.h"

#include "material.h"

namespace nau
{
    /**
     */
    struct NAU_ABSTRACT_TYPE IMaterialAssetAccessor : IAssetAccessor
    {
        NAU_INTERFACE(nau::IMaterialAssetAccessor, IAssetAccessor)

        virtual Result<> fillMaterial(Material& material) const = 0;
    };
} // namespace nau
