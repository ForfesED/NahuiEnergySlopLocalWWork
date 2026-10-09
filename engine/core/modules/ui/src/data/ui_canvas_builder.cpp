// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/ui/data/ui_canvas_builder.h"

#include "nau/ui/assets/ui_asset.h"

namespace nau::ui
{
    async::Task<> UiCanvasBuilder::loadIntoScene(nau::ui::Canvas* uiCanvas, UiSceneAssetRef assetRef)
    {
        nau::Ptr<data::UiAssetView> uiAsset = co_await assetRef.getAssetViewTyped<data::UiAssetView>();

        if (uiAsset)
        {
            uiAsset->createUi(uiCanvas);
        }

        co_return;
    }

    async::Task<> UiCanvasBuilder::loadIntoScene(nau::ui::Canvas* uiCanvas, nau::Ptr<data::UiAssetView> uiAsset)
    {
        if (uiAsset)
        {
            uiAsset->createUi(uiCanvas);
        }

        co_return;
    }
} // namespace nau::ui
