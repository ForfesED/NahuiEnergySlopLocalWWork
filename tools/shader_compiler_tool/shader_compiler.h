// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <filesystem>
#include <memory>

#include "nau/assets/shader_asset_accessor.h"
#include "nau/memory/bytes_buffer.h"
#include "nau/utils/result.h"

namespace fs = std::filesystem;

namespace nau
{
    class ShaderCompiler final
    {
    public:
        ShaderCompiler();
        ~ShaderCompiler() noexcept;

        Result<> loadFile(const fs::path& filename);
        Result<Shader> getResult() const;

        Result<> compile(
            ShaderTarget stage,
            std::string_view entry,
            const std::vector<std::wstring>& defines,
            const std::vector<std::wstring>& includeDirs,
            const std::optional<fs::path>& pdbFilename,
            bool needEmdedDebug);

        void reset();

        Result<BytesBuffer> getBytecode() const;
        Result<ShaderReflection> getReflection() const;

    private:
        std::unique_ptr<class ShaderCompilerImpl> m_pimpl = nullptr;
    };
} // namespace nau
