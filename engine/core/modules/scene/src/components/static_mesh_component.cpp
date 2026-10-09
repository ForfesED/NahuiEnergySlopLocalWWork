// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/components/static_mesh_component.h"

namespace nau::scene
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(StaticMeshComponent)

    StaticMeshAssetRef StaticMeshComponent::getMeshGeometry() const
    {
        return m_geometryAsset;
    }

    void StaticMeshComponent::setMeshGeometry(StaticMeshAssetRef assetRef)
    {
        m_geometryAsset = std::move(assetRef);
    }

    MaterialAssetRef& StaticMeshComponent::getMaterial() const
    {
        return m_materialAsset;
    }

    void StaticMeshComponent::setMaterial(const MaterialAssetRef& assetRef)
    {
        m_materialAsset = assetRef;
        m_dirtyFlags |= static_cast<uint32_t>(DirtyFlags::Material);
    }

    uint32_t StaticMeshComponent::getDirtyFlags() const
    {
        return m_dirtyFlags;
    }

    void StaticMeshComponent::resetDirtyFlags()
    {
        m_dirtyFlags = 0;
    }

    bool StaticMeshComponent::getVisibility()
    {
        return m_isVisible;
    }

    void StaticMeshComponent::setVisibility(bool isVisible)
    {
        m_isVisible = isVisible;
        m_dirtyFlags |= static_cast<uint32_t>(DirtyFlags::Visibility);
    }

    bool StaticMeshComponent::getCastShadow()
    {
        return m_castShadow;
    }

    void StaticMeshComponent::setCastShadow(bool castShadow)
    {
        m_castShadow = castShadow;
        m_dirtyFlags |= static_cast<uint32_t>(DirtyFlags::CastShadow);
    }

    void StaticMeshComponent::notifyTransformChanged()
    {
        SceneComponent::notifyTransformChanged();
        m_dirtyFlags |= static_cast<uint32_t>(DirtyFlags::WorldPos);
    }

    void StaticMeshComponent::notifyTransformChanged(const math::Transform& worldTransformCache)
    {
        SceneComponent::notifyTransformChanged(worldTransformCache);
        m_dirtyFlags |= static_cast<uint32_t>(DirtyFlags::WorldPos);
    }

}  // namespace nau
