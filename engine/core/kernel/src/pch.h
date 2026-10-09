// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <nau/core_defines.h>

#ifdef NAU_PLATFORM_WIN32
    #include "nau/platform/windows/windows_headers.h"
#endif

#include <EASTL/array.h>
#include <EASTL/atomic.h>
#include <EASTL/chrono.h>
#include <EASTL/intrusive_list.h>
#include <EASTL/list.h>
#include <EASTL/map.h>
#include <EASTL/memory.h>
#include <EASTL/optional.h>
#include <EASTL/set.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/string.h>
#include <EASTL/string_view.h>
#include <EASTL/tuple.h>
#include <EASTL/type_traits.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unordered_set.h>
#include <EASTL/variant.h>
#include <EASTL/vector.h>

#include <algorithm>
#include <filesystem>
#include <format>
#include <mutex>
#include <optional>
#include <regex>
#include <shared_mutex>
#include <thread>

#include "nau/threading/lock_guard.h"
#include "nau/utils/scope_guard.h"
