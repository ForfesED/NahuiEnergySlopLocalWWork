// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_container.h"
#include "nau/assets/ui_asset_accessor.h"

namespace nau
{
    /**
     */
    class NanimAssetContainerLoader final : public IAssetContainerLoader
    {
        NAU_INTERFACE(nau::NanimAssetContainerLoader, IAssetContainerLoader)

    public:
        NanimAssetContainerLoader() = default;

    private:
        virtual eastl::vector<eastl::string_view> getSupportedAssetKind() const override;

        virtual async::Task<IAssetContainer::Ptr> loadFromStream(io::IStreamReader::Ptr file, AssetContentInfo info) override;

        virtual RuntimeReadonlyDictionary::Ptr getDefaultImportSettings() const override;
    };

    class NanimStreamAssetContainer final : public IAssetContainer
    {
        NAU_CLASS_(nau::NanimStreamAssetContainer, IAssetContainer)

    public:
        NanimStreamAssetContainer(io::IStreamReader::Ptr stream);

        io::IStreamReader::Ptr getStream();

    private:
        virtual nau::Ptr<> getAsset(eastl::string_view path) override;
        virtual eastl::vector<eastl::string> getContent() const override;

    private:
        io::IStreamReader::Ptr m_stream;
        nau::Ptr<> m_assetInstance;
    };
}  // namespace nau
