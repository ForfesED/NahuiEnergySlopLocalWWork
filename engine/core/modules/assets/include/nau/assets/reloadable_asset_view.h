// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// core/assets/asset_view.h


#pragma once

#include "nau/assets/asset_view.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/rtti/weak_ptr.h"
#include "nau/threading/lock_guard.h"
#include "nau/threading/spin_lock.h"

namespace nau
{
    class AssetDescriptorImpl;
    class AssetViewEntry;

    struct NAU_COREASSETS_EXPORT ReloadableAssetView : virtual IRefCounted
    {
        NAU_CLASS_(nau::ReloadableAssetView, IRefCounted)

        template <::nau::RefCountedConcept>
        friend struct ::nau::rtti_detail::RttiClassStorage;

        using Ptr = nau::Ptr<nau::ReloadableAssetView>;
        using AssetViewPtr = nau::Ptr<IAssetView>;

    private:
        threading::SpinLock m_mutex;
        nau::Ptr<IAssetView> m_assetView = nullptr;

        ReloadableAssetView() = default;
        ReloadableAssetView(AssetViewPtr assetView);

        void reloadAssetView(AssetViewPtr newAssetView);

        friend AssetDescriptorImpl;
        friend AssetViewEntry;

    public:
        ReloadableAssetView(nullptr_t);
        ReloadableAssetView(const ReloadableAssetView& reloadableAssetView) = delete;
        ReloadableAssetView(ReloadableAssetView&& reloadableAssetView) = delete;
        ~ReloadableAssetView() = default;

        AssetViewPtr get();

        template <std::derived_from<IAssetView> AssetViewType>
        void getTyped(nau::Ptr<AssetViewType>& ptr)
        {
            lock_(m_mutex);

            if (!m_assetView || !m_assetView->is<AssetViewType>())
            {
                ptr = nullptr;
            }

            ptr = m_assetView;
        };
    };
}  // namespace nau
