// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/async/task.h"
#include "nau/rtti/type_info.h"

namespace nau
{
    /**
     */
    struct NAU_ABSTRACT_TYPE IAsyncDisposable
    {
        NAU_TYPEID(nau::IAsyncDisposable)

        virtual async::Task<> disposeAsync() = 0;
    };

}  // namespace nau
