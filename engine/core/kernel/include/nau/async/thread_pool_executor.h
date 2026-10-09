// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <optional>

#include "nau/async/executor.h"
#include "nau/kernel/kernel_config.h"

namespace nau::async
{
    NAU_KERNEL_EXPORT Executor::Ptr createThreadPoolExecutor(std::optional<size_t> threadsCount = std::nullopt);

    // NAU_KERNEL_EXPORT Executor::Ptr createDagThreadPoolExecutor(bool initCpuJobs, std::optional<size_t> threadsCount = std::nullopt);

}  // namespace nau::async
