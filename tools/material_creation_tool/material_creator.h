// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <EASTL/optional.h>

#include "nau/assets/material.h"

namespace nau
{
    class MaterialCreator final
    {
    public:
        Result<> createMaterial(eastl::string_view name);
        Result<> addPipeline(eastl::string_view name, eastl::string_view shaderCacheFilename, const eastl::vector<Shader>& shaders);

        Result<Material> getResult() const;

        void clear();

    private:
        void processCbuffer(MaterialPipeline& pipeline, const ShaderInputBindDescription& bind);
        void processTexture(MaterialPipeline& pipeline, const ShaderInputBindDescription& bind);
        void processSampler(MaterialPipeline& pipeline, const ShaderInputBindDescription& bind);

        eastl::optional<Material> m_material = eastl::nullopt;
    };
} // namespace nau
