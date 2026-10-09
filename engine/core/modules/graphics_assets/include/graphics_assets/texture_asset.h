// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/3d/dag_drv3d.h"
#include "nau/assets/asset_view.h"
#include "nau/rtti/rtti_impl.h"

namespace nau
{
    class NAU_GRAPHICSASSETS_EXPORT TextureAssetView : public IAssetView
    {
        NAU_CLASS_(nau::TextureAssetView, IAssetView)
    public:
        static async::Task<nau::Ptr<TextureAssetView>> createFromAssetAccessor(nau::Ptr<> accessor);

        inline BaseTexture* getTexture()
        {
            return m_Texture;
        }

        using Ptr = nau::Ptr<TextureAssetView>;
    private:
        BaseTexture* m_Texture;
    };
}  // namespace nau
