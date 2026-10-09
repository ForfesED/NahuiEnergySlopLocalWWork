// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/kernel/kernel_config.h"
#include "nau/rtti/type_info.h"
#include "nau/serialization/runtime_value.h"

namespace nau::rtti
{
    struct NAU_ABSTRACT_TYPE RuntimeTypeInfoValue : virtual RuntimePrimitiveValue
    {
        NAU_INTERFACE(nau::rtti::RuntimeTypeInfoValue, RuntimePrimitiveValue)

        virtual const TypeInfo& getTypeInfo() const = 0;

        virtual void setTypeInfo(const TypeInfo& type) = 0; 
    };


    NAU_KERNEL_EXPORT RuntimeValue::Ptr makeValueRef(TypeInfo& value, IMemAllocator::Ptr allocator = nullptr);

    NAU_KERNEL_EXPORT RuntimeValue::Ptr makeValueRef(const TypeInfo& value, IMemAllocator::Ptr allocator = nullptr);

    NAU_KERNEL_EXPORT RuntimeValue::Ptr makeValueCopy(TypeInfo value, IMemAllocator::Ptr allocator = nullptr);
}  // namespace nau::rtti
