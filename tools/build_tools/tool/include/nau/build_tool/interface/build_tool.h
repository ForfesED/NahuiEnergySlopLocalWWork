// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "nau/build_tool/build_config.h"

namespace nau
{
    typedef std::function<void(enum class BuildResult, const std::string&)> BuildResultCallback;
    typedef std::function<void(int)> ProgressCallback;

    class BUILD_TOOL_API IBuildTool
    {
    public:
        virtual void build(const struct BuildConfig& config, ProgressCallback& progressCallback, BuildResultCallback& resultCallback) = 0;
        virtual bool compile(const BuildConfig& config, ProgressCallback& progressCallback, BuildResultCallback& resultCallback) = 0;
        virtual void cancel() = 0;
        static std::shared_ptr<IBuildTool> get();
    };
}  // namespace nau