// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/debug/debug_utils.h



#pragma once

#include "nau/kernel/kernel_config.h"
#include "nau/utils/preprocessor.h"

#include NAU_PLATFORM_HEADER(platform_debug.h)

namespace nau::debug
{
    NAU_KERNEL_EXPORT
    bool isRunningUnderDebugger();

    inline void debugBreak()
    {
        NAU_PLATFORM_BREAK;
    }

} // namespace nau::debug
