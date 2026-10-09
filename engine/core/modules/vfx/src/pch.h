// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/core_defines.h"

#ifdef NAU_PLATFORM_WIN32
    #include "nau/platform/windows/windows_headers.h"
#endif

// clang-format off

#include <EASTL/string.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/vector.h>
#include <EASTL/array.h>
#include <EASTL/map.h>

// clang-format on

#include <atomic>
#include <memory>
#include <mutex>

#include "nau/rtti/ptr.h"
#include "nau/service/service.h"
#include "nau/service/service_provider.h"
#include "nau/threading/lock_guard.h"


