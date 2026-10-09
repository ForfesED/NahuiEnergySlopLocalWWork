// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/assets/reloadable_asset_view.h"

namespace nau
{

    ReloadableAssetView::ReloadableAssetView(ReloadableAssetView::AssetViewPtr assetView) :
        m_assetView(assetView)
    {
    }

    void ReloadableAssetView::reloadAssetView(ReloadableAssetView::AssetViewPtr newAssetView)
    {
        lock_(m_mutex);
        m_assetView = newAssetView;
    }

    ReloadableAssetView::ReloadableAssetView(nullptr_t) :
        m_assetView(nullptr)
    {
    }

    ReloadableAssetView::AssetViewPtr ReloadableAssetView::get()
    {
        lock_(m_mutex);
        return m_assetView;
    }
}  // namespace nau
