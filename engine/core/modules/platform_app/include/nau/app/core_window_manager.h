// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/app/core_window_manager.h


#pragma once

#include <chrono>
#include <optional>

#include "nau/app/platform_window.h"
#include "nau/app/window_manager.h"
#include "nau/async/executor.h"
#include "nau/kernel/kernel_config.h"
#include "nau/rtti/ptr.h"
#include "nau/utils/result.h"


namespace nau
{
    struct NAU_ABSTRACT_TYPE ICoreWindowManager : IWindowManager
    {
        NAU_INTERFACE(nau::ICoreWindowManager, IWindowManager)

        virtual void bindToCurrentThread() = 0;

        virtual async::Executor::Ptr getExecutor() = 0;

        virtual Result<> pumpMessageQueue(bool waitForMessage, std::optional<std::chrono::milliseconds> maxProcessingTime = std::nullopt) = 0;

        virtual nau::Ptr<IPlatformWindow> createWindow(bool exitAppOnClose = false) = 0;
    };

}  // namespace nau
