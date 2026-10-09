// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "shader_compiler.h"

#include "nau/assets/shader_asset_accessor.h"
#include "nau/io/stream.h"
#include "nau/utils/functor.h"
#include "nau/utils/result.h"
#include "nau/usd_meta_tools/usd_meta_info.h"

#include <filesystem>
#include "shader_cache.h"

namespace fs = std::filesystem;

namespace nau
{
    class ShaderCacheBuilderFromAsset final : public IShaderCache
    {
    public:

        Result<> makeCache(StreamFactory streamFactory, const Arguments& args) override;
        Result<> makeCacheFiles(StreamFactory streamFactory, const Arguments& args) override;

    private:

        Result<std::vector<fs::path>> collectShaderInfo(const fs::path& metafilesPath);
        Result<std::vector<fs::path>> collectFiles(const fs::path& directory, std::string_view extension);
        Result<std::vector<Shader>> compileShaders(const std::vector<fs::path>& shaderInfos, const Arguments& args);

        nau::Result<std::vector<Shader>> compileShader(ShaderCompiler* compiler, 
            const nau::UsdMetaInfo& meta,
            const std::vector<std::wstring>& includeDirs, 
            const std::optional<fs::path>& pdbDir,
            bool needEmbedDebug);
    };
} // namespace nau
