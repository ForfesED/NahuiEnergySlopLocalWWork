// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <filesystem>
#include "nau/io/stream.h"
#include "nau/utils/functor.h"
#include "nau/utils/result.h"


namespace fs = std::filesystem;

namespace nau
{

    class IShaderCache
    {
    public:

        using StreamFactory = Functor<io::IStreamWriter::Ptr(std::string_view shaderName)>;

        struct Arguments
        {
            fs::path outDir;
            fs::path shadersPath;
            fs::path metafilesPath;
            std::string shaderCacheName;
            std::vector<fs::path> includeDirs;
            std::optional<fs::path> debugOutputDir;
            bool embedDebugInfo;
        };


        IShaderCache() = default;
        virtual ~IShaderCache() = default;

        virtual Result<> makeCache(StreamFactory streamFactory, const Arguments& args) = 0;
        virtual Result<> makeCacheFiles(StreamFactory streamFactory, const Arguments& args) = 0;

    };


}