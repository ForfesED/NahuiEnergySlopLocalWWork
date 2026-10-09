// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/assets/asset_ref.h"

namespace nau::ui
{
    class Canvas;

    namespace data
    {
        class UiAssetView;
    }

    class NAU_UI_EXPORT UiCanvasBuilder
    {
    public:
        static async::Task<> loadIntoScene(Canvas* uiCanvas, nau::Ptr<data::UiAssetView> uiAsset);
        static async::Task<> loadIntoScene(Canvas* uiCanvas, UiSceneAssetRef assetRef);
    };
} // namespace nau::ui
