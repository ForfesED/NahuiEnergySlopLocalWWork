// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "graphics_assets/material_asset.h"
#include "graphics_assets/shader_asset.h"
#include "nau/assets/asset_ref.h"

namespace nau::render
{
    class NAU_GRAPHICS_EXPORT EnvironmentRenderer
    {
    public:
        explicit EnvironmentRenderer(MaterialAssetView::Ptr envCubemapMaterial
            , ShaderAssetView::Ptr panoramaToCubemapComputeShader
            , ShaderAssetView::Ptr genIrradianceMapComputeShader
            , ShaderAssetView::Ptr genReflectionMapComputeShader);
        ~EnvironmentRenderer();

        void setPanoramaTexture(ReloadableAssetView::Ptr panoramaTex);

        void renderSkybox(Texture* renderTargetHDR, Texture* sceneDepth, const nau::math::Matrix4& viewMatrix, const nau::math::Matrix4& projMatrix) const;

        void setEnvCubemapsDirty(bool value);
        bool isEnvCubemapsDirty() const;

        void convertPanoramaToCubemap();

        void generateIrradianceMap();
        void generateReflectionMap();

        CubeTexture* getEnvCubemap() const;

        CubeTexture* getIrradianceMap() const;
        CubeTexture* getReflectionMap() const;

    protected:
        void createSkyboxIndexBuffer();

        bool m_envCubemapsDirty = true;

        MaterialAssetView::Ptr m_envCubemapMaterial;
        Sbuffer* m_envCubemapIndexBuffer = nullptr;

        TextureAssetView::Ptr m_panoramaTextureViewCached;
        ReloadableAssetView::Ptr m_panoramaTextureView;
        d3d::SamplerHandle m_csTexSampler;

        ShaderAssetView::Ptr m_panoramaToCubemapCS;
        ShaderAssetView::Ptr m_genIrradianceMapCS;
        ShaderAssetView::Ptr m_genReflectionMapCS;

        PROGRAM m_panoramaToCubemapCSProgram;
        PROGRAM m_genIrradianceMapCSProgram;
        PROGRAM m_genReflectionMapCSProgram;

        CubeTexture* m_envCubemapTexture;
        CubeTexture* m_irradianceMap;
        CubeTexture* m_reflectionMap;
    };

}  // namespace nau::render