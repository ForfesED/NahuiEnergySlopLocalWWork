// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/rtti/ptr.h"
#include "nau/rtti/rtti_object.h"

namespace nau
{
    /**
     * @brief Provides generic interface for accessing assets.
     * 
     * Implementations are to provide concrete functions for accessing supported types of assets.
     */
    struct NAU_ABSTRACT_TYPE IAssetAccessor : IRefCounted
    {
        NAU_INTERFACE(nau::IAssetAccessor, IRefCounted)

        using Ptr = nau::Ptr<IAssetAccessor>;
    };

}  // namespace nau
