// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/components/environment_component.h"

namespace nau::scene
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(EnvironmentComponent);

    TextureAssetRef EnvironmentComponent::getTextureAsset() const
    {
        return m_textureAsset;
    }

    void EnvironmentComponent::setTextureAsset(const TextureAssetRef& texture)
    {
        m_textureAsset = texture;
        m_isTextureDirty = true;
    }

    bool EnvironmentComponent::isTextureDirty() const
    {
        return m_isTextureDirty;
    }

    void EnvironmentComponent::resetIsTextureDirty()
    {
        m_isTextureDirty = false;
    }

    void EnvironmentComponent::setIntensity(float intensity)
    {
        m_enviIntensity = intensity;
    }

    float EnvironmentComponent::getIntensity() const
    {
        return m_enviIntensity;
    }

}  // namespace nau::scene
