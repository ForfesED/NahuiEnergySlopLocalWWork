// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// pch.cpp


#pragma once

#include <nau/core_defines.h>

#ifdef NAU_PLATFORM_WIN32
    #include "nau/platform/windows/windows_headers.h"
#endif

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>
