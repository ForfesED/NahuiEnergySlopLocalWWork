// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_ref.h"
#include "nau/scene/components/component_attributes.h"
#include "nau/scene/components/internal/component_internal_attributes.h"
#include "nau/scene/components/scene_component.h"

namespace nau::scene
{
    class NAU_CORESCENE_EXPORT StaticMeshComponent : public SceneComponent
    {
        NAU_OBJECT(nau::scene::StaticMeshComponent, SceneComponent)
        NAU_DECLARE_DYNAMIC_OBJECT

        NAU_CLASS_ATTRIBUTES(
            CLASS_ATTRIBUTE(SystemComponentAttrib, true),
            CLASS_ATTRIBUTE(ComponentDisplayNameAttrib, "Static Mesh"),
            CLASS_ATTRIBUTE(ComponentDescriptionAttrib, "Static Mesh (description)"))

        NAU_CLASS_FIELDS(
            CLASS_NAMED_FIELD(m_geometryAsset, "geometry"),
            CLASS_NAMED_FIELD(m_materialAsset, "material"),
            CLASS_NAMED_FIELD(m_isVisible, "is visible"),
            CLASS_NAMED_FIELD(m_castShadow, "cast shadow"))

    public:
        enum class DirtyFlags : uint32_t
        {
            WorldPos    = 1 << 0,
            Visibility  = 1 << 1,
            Material    = 1 << 2,
            CastShadow  = 1 << 3,
        };

        StaticMeshAssetRef getMeshGeometry() const;

        // Due to a commit that broke the copy constructor, we will pass arguments by const reference,
        // as the copy assignment operator is still working.
        // TODO: Revert to passing by value when the copy constructor is fixed, or just remove this comment.
        void setMeshGeometry(StaticMeshAssetRef assetRef);

        MaterialAssetRef& getMaterial() const;

        // Due to a commit that broke the copy constructor, we will pass arguments by const reference,
        // as the copy assignment operator is still working.
        // TODO: Revert to passing by value when the copy constructor is fixed, or just remove this comment.
        void setMaterial(const MaterialAssetRef& assetRef);

        uint32_t getDirtyFlags() const;
        void resetDirtyFlags();

        bool getVisibility();
        void setVisibility(bool isVisible);

        bool getCastShadow();
        void setCastShadow(bool castShadow);

    protected:
        void notifyTransformChanged() override;
        void notifyTransformChanged(const math::Transform& worldTransformCache) override;

    private:
        mutable StaticMeshAssetRef m_geometryAsset;
        mutable MaterialAssetRef m_materialAsset;
        bool m_castShadow = true;
        bool m_isVisible = true;
        uint32_t m_dirtyFlags = 0;
    };
}  // namespace nau::scene
