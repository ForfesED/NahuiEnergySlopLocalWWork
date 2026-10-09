// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/components/skinned_mesh_component.h"

namespace nau
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(SkinnedMeshComponent)

    SkinnedMeshAssetRef& SkinnedMeshComponent::getMeshGeometry() const
    {
        return m_geometryAsset;
    }

    void SkinnedMeshComponent::setMeshGeometry(const SkinnedMeshAssetRef& asset)
    {
        m_geometryAsset = std::move(asset);
    }

    MaterialAssetRef& SkinnedMeshComponent::getMaterial() const
    {
        return m_materialAsset;
    }

    void SkinnedMeshComponent::setMaterial(const MaterialAssetRef& assetRef)
    {
        m_materialAsset = assetRef;
        m_isMaterialDirty = true;
    }

    bool SkinnedMeshComponent::isMaterialDirty() const
    {
        return m_isMaterialDirty;
    }

    void SkinnedMeshComponent::resetIsMaterialDirty()
    {
        m_isMaterialDirty = false;
    }

}  // namespace nau
