// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/dispatch/dynamic_object.h


#pragma once


#include "nau/dispatch/class_descriptor.h"
#include "nau/rtti/ptr.h"
#include "nau/serialization/runtime_value.h"

namespace nau
{
    /**
     */
    struct NAU_ABSTRACT_TYPE DynamicObject : virtual RuntimeObject
    {
        NAU_INTERFACE(nau::DynamicObject, RuntimeObject)

        using Ptr = nau::Ptr<DynamicObject>;

        virtual IClassDescriptor::Ptr getClassDescriptor() const = 0;

    };
}  // namespace nau

