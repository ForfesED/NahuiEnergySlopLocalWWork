// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// core/assets/asset_view.h


#pragma once
#include "nau/rtti/rtti_object.h"
#include "nau/rtti/ptr.h"

namespace nau
{
    /**
     * @brief Provides an interface for an asset view.
     * 
     * A view is an interpretation of a collection of data, i.e. an asset or a resource.
     * An asset may have several different views each providing various ways of addressing the asset. 
     * An example is a texture resource in rendering. A texture in DirectX render framework can be a shader resource (that is, input data for a shader)
     * or a render target (that is, an output for a shader). Its usages may switch in-between draw calls. Each use case is supplied by a corresponding view,
     * however the actual resource, i.e. a dedicated space in memory, that is to say a collection of bytes, is the same.
     */
    struct NAU_ABSTRACT_TYPE IAssetView : virtual IRefCounted
    {
        NAU_INTERFACE(nau::IAssetView, IRefCounted)

        using Ptr = nau::Ptr<IAssetView>;
    };
}
