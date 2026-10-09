// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <EASTL/string.h>
#include <EASTL/string_view.h>
#include <EASTL/vector.h>
#include <pxr/usd/usd/prim.h>

#include "nau/assets/asset_container.h"
#include "nau/rtti/rtti_impl.h"

namespace UsdTranslator
{
    /**
     */
    class UsdMeshContainer final : public nau::IAssetContainer
    {
        NAU_CLASS_(UsdTranslator::UsdMeshContainer, nau::IAssetContainer);

    public:
        UsdMeshContainer(PXR_NS::UsdPrim prim);

        nau::Ptr<> getAsset(eastl::string_view path) override;

        eastl::vector<eastl::string> getContent() const override;

    private:
        PXR_NS::UsdPrim m_prim;
    };

}  // namespace UsdTranslator
