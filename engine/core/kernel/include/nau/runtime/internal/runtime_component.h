// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/runtime/runtime_component.h


#pragma once

#include "nau/rtti/rtti_object.h"

namespace nau
{

    /**
     */
    struct NAU_ABSTRACT_TYPE IRuntimeComponent : virtual IRttiObject
    {
        NAU_INTERFACE(nau::IRuntimeComponent, IRttiObject)

        virtual bool hasWorks() = 0;
    };

}  // namespace nau
