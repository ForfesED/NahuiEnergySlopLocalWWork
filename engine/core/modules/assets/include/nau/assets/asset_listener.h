// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_descriptor.h"
#include "nau/assets/asset_view.h"
#include "nau/async/task_base.h"
#include "nau/rtti/type_info.h"
#include "nau/utils/functor.h"

namespace nau
{
    /**
        @brief API to receive assets system events.
     */
    struct IAssetListener
    {
        NAU_TYPEID(nau::IAssetListener);

        using LockAssetFunctor = Functor<async::Task<>()>;

        virtual void onAssetLoad(IAssetDescriptor::AssetId assetId) = 0;

        virtual void onAssetUnload(IAssetDescriptor::AssetId assetId) = 0;

        /**
            @brief Is called by the system when the asset view needs to be updated.
         */
        virtual async::Task<> onAssetViewUpdate(IAssetDescriptor::AssetId assetId, IAssetView::Ptr oldAssetView, IAssetView::Ptr newAssetView) = 0;
    };
}  // namespace nau
