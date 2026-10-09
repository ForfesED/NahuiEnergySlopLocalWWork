// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <map>
#include <string>
#include <vector>

#include "nau/shared/interface/job.h"
#include "nau/project_tools/project_api.h"

namespace nau
{
    class PROJECT_TOOL_API NauInitProjectJob final : public Job
    {
    public:
        virtual int run(const struct CommonArguments* const) override;
    };

    class PROJECT_TOOL_API NauRebuildProjectJob final : public Job
    {
    public:
        virtual int run(const struct CommonArguments* const) override;
    };

    class PROJECT_TOOL_API NauUpgradeProjectJob final : public Job
    {
    public:
        virtual int run(const struct CommonArguments* const) override;
    };

    class PROJECT_TOOL_API NauSaveProjectJob final : public Job
    {
    public:
        virtual int run(const struct CommonArguments* const) override;
    };
}