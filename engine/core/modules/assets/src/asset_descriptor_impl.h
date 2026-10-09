// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_accessor.h"
#include "nau/assets/asset_container.h"
#include "nau/assets/asset_descriptor.h"
#include "nau/assets/asset_view.h"
#include "nau/assets/reloadable_asset_view.h"
#include "nau/assets/internal/asset_descriptor_inernal.h"
#include "nau/async/multi_task_source.h"
#include "nau/memory/eastl_aliases.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/rtti/type_info.h"
#include "nau/rtti/weak_ptr.h"
#include "nau/threading/spin_lock.h"
#include "nau/utils/functor.h"

namespace nau
{
    /**
     */
    class AssetDescriptorImpl : public IAssetDescriptor,
                                public assets::IAssetDescriptorInternal
    {
        NAU_CLASS_(nau::AssetDescriptorImpl, IAssetDescriptor, assets::IAssetDescriptorInternal)

    public:
        using ContainerLoaderFunc = Functor<async::Task<IAssetContainer::Ptr>()>;

        ~AssetDescriptorImpl();
        AssetDescriptorImpl(AssetPath assetPath, ContainerLoaderFunc loader);

        AssetDescriptorImpl(const AssetDescriptorImpl&) = delete;
        AssetDescriptorImpl& operator=(const AssetDescriptorImpl&) = delete;

        IAssetDescriptor::Ptr getInnerAsset(eastl::string_view);

        IAssetContainer* getLoadedContainer();

        AssetId getAssetId() const final;

        AssetPath getAssetPath() const override;

        async::Task<IAssetView::Ptr> getAssetView(const AssetViewDescription&) override;
        async::Task<ReloadableAssetView::Ptr> getReloadableAssetView(const AssetViewDescription&) override;

        async::Task<nau::Ptr<>> getRawAsset() override;

        void load() override;

        IAssetDescriptor::UnloadResult unload() override;

        LoadState getLoadState() const override;


    private:
        class AssetViewEntry
        {
        public:
            AssetViewEntry() = delete;
            AssetViewEntry(eastl::string assetInnerPath, const rtti::TypeInfo* viewType);
            AssetViewEntry(const AssetViewEntry&) = delete;
            ~AssetViewEntry();

            AssetViewEntry& operator=(const AssetViewEntry&) = delete;

            bool isThatView(eastl::string_view innerPath, const rtti::TypeInfo* viewType) const;

            bool hasNoAssetViewReferences();

            async::Task<IAssetView::Ptr> getAssetView(IAssetContainer& container);
            async::Task<ReloadableAssetView::Ptr> getReloadableAssetView(IAssetContainer& container);

            async::Task<> updateAssetView(IAssetDescriptor::AssetId assetId, IAssetContainer& container);

            IAssetView::Ptr getFabricatedAssetView();

        private:

            async::Task<IAssetView::Ptr> fabricateAssetView(IAssetContainer& container);

            const eastl::string m_assetInnerPath;
            const rtti::TypeInfo* const m_viewType = nullptr;

            nau::WeakPtr<IAssetView> m_assetViewRef;
            nau::WeakPtr<ReloadableAssetView> m_reloadableAssetViewRef;
            async::MultiTaskSource<IAssetView::Ptr> m_assetViewCreationState = nullptr;
            threading::SpinLock m_mutex;
        };

        async::Task<IAssetContainer::Ptr> getContainer();

        async::Task<IAssetView::Ptr> getInnerAssetView(eastl::string_view innerPath, const rtti::TypeInfo* viewType);
        async::Task<ReloadableAssetView::Ptr> getInnerReloadableAssetView(eastl::string_view innerPath, const rtti::TypeInfo* viewType);

        async::Task<nau::Ptr<>> getInnerRawAsset(eastl::string_view innerPath);

        eastl::optional<assets::AssetInternalState> getInnerCachedAssetViewState(eastl::string_view innerPath, const rtti::TypeInfo* viewType, assets::InternalStateOptsFlag);

        eastl::optional<assets::AssetInternalState> getCachedAssetViewInternalState(const rtti::TypeInfo* viewType, assets::InternalStateOptsFlag) override;


        const AssetId m_assetId = 0;
        const AssetPath m_assetPath;
        ContainerLoaderFunc m_containerLoader;
        IAssetContainer::Ptr m_container;
        async::MultiTaskSource<IAssetContainer::Ptr> m_containerLoadingState = nullptr;
        eastl::list<AssetViewEntry, EastlBlockAllocator<alignedSize(sizeof(AssetViewEntry), 64)>> m_assetViews;
        mutable std::mutex m_mutex;

        friend class InnerAssetDescriptor;
    };

}  // namespace nau
