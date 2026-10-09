// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/assets/asset_container.h"
#include "nau/assets/asset_container_builder.h"
#include "nau/io/file_system.h"


namespace nau::scene
{
    async::Task<> sampleStoreAsset()
    {
        using namespace nau::io;

        IAssetContainerLoader* textureLoader = nullptr;

        for (auto* const loader : getServiceProvider().getAll<IAssetContainerLoader>())
        {
            eastl::vector<eastl::string_view> supportedAssetKind = loader->getSupportedAssetKind();
            if (std::find(supportedAssetKind.begin(), supportedAssetKind.end(), eastl::string_view{"png"}) != supportedAssetKind.end())
                textureLoader = loader;
            break;
        }

        NAU_ASSERT(textureLoader != nullptr);

        auto& fileSystem = getServiceProvider().get<IFileSystem>();
        auto file = fileSystem.openFile("/content/textures/white_8x8.png", AccessMode::Read, OpenFileMode::OpenExisting);

        IAssetContainer::Ptr originalAssetContainer = co_await textureLoader->loadFromStream(file->createStream(), {"png", "", textureLoader->getDefaultImportSettings()});
        NAU_ASSERT(originalAssetContainer);

        nau::Ptr<> asset = originalAssetContainer->getAsset();
        NAU_ASSERT(asset);

        IAssetContainerBuilder& builder = getServiceProvider().get<IAssetContainerBuilder>();

        IStreamWriter::Ptr stream = io::createNativeFileStream("white_8x8.dds", io::AccessMode::Write, io::OpenFileMode::CreateAlways);
        builder.writeAssetToStream(stream, asset).ignore();
    }
}
