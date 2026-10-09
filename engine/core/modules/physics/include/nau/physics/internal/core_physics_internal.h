// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/async/executor.h"
#include "nau/rtti/type_info.h"

namespace nau::physics
{
    /**
     */
    struct NAU_ABSTRACT_TYPE ICorePhysicsInternal
    {
        NAU_TYPEID(nau::physics::ICorePhysicsInternal)

        virtual ~ICorePhysicsInternal() = default;

        virtual async::Executor::Ptr getExecutor() = 0;
    };
}  // namespace nau::physics
