// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#if defined(_MSC_VER)
#if !__cpp_impl_coroutine
#error "Require coroutine implementation"
#endif

#include <coroutine>

namespace CoroNs = std;

#else

#error Setup coroutine

#endif



