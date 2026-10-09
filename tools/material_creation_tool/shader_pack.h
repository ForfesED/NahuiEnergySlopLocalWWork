// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <filesystem>

#include "nau/assets/shader.h"
#include "nau/io/file_system.h"

namespace fs = std::filesystem;

namespace nau
{
    class ShaderPack
    {
    public:
        explicit ShaderPack(io::IStreamReader::Ptr stream);

        Result<Shader> getShader(eastl::string_view name);
        Result<eastl::vector<Shader>> getShaders(const eastl::vector<eastl::string>& names);

    private:
        struct ShaderBytecodeEntry
        {
            eastl::string shaderName;
            size_t blobOffset;
            size_t blobSize;

            NAU_CLASS_FIELDS(
                CLASS_FIELD(shaderName),
                CLASS_FIELD(blobOffset),
                CLASS_FIELD(blobSize)
            )
        };

        struct ShaderPackContainerData
        {
            eastl::vector<Shader> shaders;
            eastl::vector<ShaderBytecodeEntry> byteCode;

            NAU_CLASS_FIELDS(
                CLASS_FIELD(shaders),
                CLASS_FIELD(byteCode)
            )
        };

        Shader* findShader(eastl::string_view shaderName);

        ShaderPackContainerData m_shadersPackData;
        io::IStreamReader::Ptr m_stream;
    };
} // namespace nau
