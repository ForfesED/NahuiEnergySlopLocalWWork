// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_ref.h"
#include "nau/scene/components/component_attributes.h"
#include "nau/scene/components/internal/component_internal_attributes.h"
#include "nau/scene/components/scene_component.h"

namespace nau
{
    class NAU_CORESCENE_EXPORT SkinnedMeshComponent : public scene::SceneComponent
    {
        NAU_OBJECT(nau::SkinnedMeshComponent, scene::SceneComponent)
        NAU_DECLARE_DYNAMIC_OBJECT

        NAU_CLASS_ATTRIBUTES(
            CLASS_ATTRIBUTE(scene::SystemComponentAttrib, true),
            CLASS_ATTRIBUTE(scene::ComponentDisplayNameAttrib, "Skinned Mesh"),
            CLASS_ATTRIBUTE(scene::ComponentDescriptionAttrib, "Skinned Mesh (description)"))

        NAU_CLASS_FIELDS(
            CLASS_NAMED_FIELD(m_geometryAsset, "geometry"),
            CLASS_NAMED_FIELD(m_materialAsset, "material"))

    public:
        SkinnedMeshAssetRef& getMeshGeometry() const;

        // Due to a commit that broke the copy constructor, we will pass arguments by const reference,
        // as the copy assignment operator is still working.
        // TODO: Revert to passing by value when the copy constructor is fixed, or just remove this comment.
        void setMeshGeometry(const SkinnedMeshAssetRef& assetRef);

        MaterialAssetRef& getMaterial() const;

        // Due to a commit that broke the copy constructor, we will pass arguments by const reference,
        // as the copy assignment operator is still working.
        // TODO: Revert to passing by value when the copy constructor is fixed, or just remove this comment.
        void setMaterial(const MaterialAssetRef& assetRef);

        bool isMaterialDirty() const;
        void resetIsMaterialDirty();

    private:
        mutable SkinnedMeshAssetRef m_geometryAsset;
        mutable MaterialAssetRef m_materialAsset;
        bool m_isMaterialDirty = false;
    };
}  // namespace nau
