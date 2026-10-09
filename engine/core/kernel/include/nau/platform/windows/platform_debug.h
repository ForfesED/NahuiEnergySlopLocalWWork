// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// platform/windows/platform_debug.h


#pragma once

#include <cstdlib>
#include <intrin.h>

#define NAU_PLATFORM_BREAK (__nop(), __debugbreak())
#define NAU_PLATFORM_ABORT (std::abort())
