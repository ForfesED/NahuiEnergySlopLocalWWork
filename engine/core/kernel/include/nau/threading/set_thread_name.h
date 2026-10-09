// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/threading/set_thread_name.h


#pragma once
#include <string>

#include "nau/kernel/kernel_config.h"

namespace nau::threading
{
    NAU_KERNEL_EXPORT void setThisThreadName(const std::string& name);

}  // namespace nau::threading
