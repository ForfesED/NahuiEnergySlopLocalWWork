// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_view.h"
#include "nau/async/task.h"
#include "nau/scene/scene.h"
#include "nau/assets/ui_asset_accessor.h"

namespace nau::ui
{
    class Canvas;
}

namespace nau::ui::data
{
    class NAU_UI_EXPORT UiAssetView : public IAssetView
    {
        NAU_CLASS_(nau::ui::data::UiAssetView, IAssetView)

    public:
        static nau::async::Task<nau::Ptr<UiAssetView>> createFromAssetAccessor(nau::Ptr<> accessor);

        void createUi(Canvas* uiCanvas) const;

    private:
        eastl::vector<UiElementAssetData> m_uiElementsData;
    };
} // namespace nau::ui
