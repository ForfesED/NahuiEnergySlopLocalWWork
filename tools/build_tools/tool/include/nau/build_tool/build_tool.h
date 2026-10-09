// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
#pragma once

#include <filesystem>

#include "nau/build_tool/interface/build_tool.h"
#include "nau/shared/args.h"
#include "nau/shared/version.h"

namespace nau
{
    class BuildTool final : public IBuildTool
    {
    public:
        void build(const BuildConfig& config, ProgressCallback& progressCallback, BuildResultCallback& resultCallback) override;
        // Are not to be called with build, only compiles and refreshes sources
        bool compile(const BuildConfig& config, ProgressCallback& progressCallback, BuildResultCallback& resultCallback) override;
        void cancel() override;
        bool failed() const;

    private:
        BuildResult compileSources();
        BuildResult compileAssets();
        BuildResult createPackage();

        void fail(const std::string& msg, BuildResult reason);
        void success();
        bool copyBinaries(const std::filesystem::path& buildPath, const std::filesystem::path& cMakeFiles);

        BuildResult runProcess(std::string makeArgs);

        ProgressCallback m_progressCallback;
        BuildResultCallback m_resultCallback;
        std::unique_ptr<BuildConfig> m_buildConfig;
        bool m_failed = false;

        std::atomic<bool> m_cancelled = false;
    };
}  // namespace nau