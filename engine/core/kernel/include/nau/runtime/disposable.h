// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/runtime/disposable.h


#pragma once

#include "nau/rtti/rtti_object.h"

namespace nau
{
    struct NAU_ABSTRACT_TYPE IDisposable : virtual IRttiObject
    {
        NAU_INTERFACE(nau::IDisposable, IRttiObject)

        virtual void dispose() = 0;
    };

} // namespace nau