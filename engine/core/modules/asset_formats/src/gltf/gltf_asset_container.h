// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "gltf/gltf_file.h"
#include "nau/assets/asset_container.h"
#include "nau/io/stream.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/scene/scene.h"

#include "nau/assets/scene_asset.h"

namespace nau::scene
{
    struct ISceneFactory;
}

namespace nau
{
    struct IAssetDescriptorFactory;
    /**
     */
    class GltfAssetContainer final : public IAssetContainer
    {
        NAU_CLASS_(nau::GltfAssetContainer, IAssetContainer)

    public:
        GltfAssetContainer(GltfFile gltfFile, const io::FsPath& filePath);

    private:
        nau::Ptr<> getAsset(eastl::string_view path) override;

        eastl::vector<eastl::string> getContent() const override;

        GltfFile m_gltfFile;
        eastl::vector<io::IFile::Ptr> m_bufferFiles;
        const io::FsPath m_gltfFilePath;
    };

    /**
     */
    class GltfAssetContainerLoader final : public IAssetContainerLoader
    {
        NAU_INTERFACE(nau::GltfAssetContainerLoader, IAssetContainerLoader)

    public:
        GltfAssetContainerLoader() = default;

    private:

        eastl::vector<eastl::string_view> getSupportedAssetKind() const override;

        async::Task<IAssetContainer::Ptr> loadFromStream(io::IStreamReader::Ptr stream, AssetContentInfo info) override;

        RuntimeReadonlyDictionary::Ptr getDefaultImportSettings() const override;
    };
}  // namespace nau
