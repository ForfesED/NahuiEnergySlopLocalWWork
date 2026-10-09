// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// helpers/runtime_guard.h


#pragma once
#include "EASTL/unique_ptr.h"

namespace nau::test
{
    class RuntimeGuard
    {
    public:
        using Ptr = eastl::unique_ptr<RuntimeGuard>;

        static RuntimeGuard::Ptr create();
        
        virtual ~RuntimeGuard() = default;

        virtual void reset() = 0;
    };

}
