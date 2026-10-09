// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

// clang-format off

#include "nau/diag/assertion.h"

#include "nau/serialization/native_runtime_value/native_value_forwards.h"
#include "nau/serialization/native_runtime_value/native_integer_value.h"
#include "nau/serialization/native_runtime_value/native_boolean_value.h"
#include "nau/serialization/native_runtime_value/native_float_value.h"
#include "nau/serialization/native_runtime_value/native_string_value.h"
#include "nau/serialization/native_runtime_value/native_optional_value.h"
#include "nau/serialization/native_runtime_value/native_collection.h"
#include "nau/serialization/native_runtime_value/native_tuple.h"
#include "nau/serialization/native_runtime_value/native_dictionary.h"
#include "nau/serialization/native_runtime_value/native_object.h"

// clang format on

namespace nau
{
    /**
     *
     */
    inline RuntimeValueRef::Ptr makeValueRef(const RuntimeValue::Ptr& value, IMemAllocator::Ptr allocator)
    {
        return RuntimeValueRef::create(value, std::move(allocator));
    }

    inline RuntimeValueRef::Ptr makeValueRef(RuntimeValue::Ptr& value, IMemAllocator::Ptr allocator)
    {
        return RuntimeValueRef::create(value, std::move(allocator));
    }


    template <typename T>
    Result<> runtimeValueApply(T& target, const RuntimeValue::Ptr& rtValue)
    {
        static_assert(!std::is_const_v<T>, "Const type is passed. Use remove_const_t on call site");
        return RuntimeValue::assign(makeValueRef(target), rtValue);
    }

    template <typename T>
    [[nodiscard]]
    Result<T> runtimeValueCast(const RuntimeValue::Ptr& rtValue)
    {
        static_assert(std::is_default_constructible_v<T>, "Default constructor required or use nau::RuntimeValueApply");
        static_assert(!std::is_reference_v<T>, "Reference type is passed");

        Result<std::remove_const_t<T>> value{};  // << default constructor
        NauCheckResult(runtimeValueApply(*value, rtValue));

        return value;
    }

    template<typename T>
    requires(std::is_arithmetic_v<T>)
    [[nodiscard]]
    Result<T> runtimeValueCast(const RuntimeValue::Ptr& rtValue)
    {
        if (const auto* const floatValue = rtValue->as<const RuntimeFloatValue*>())
        {
            return floatValue->get<T>();
        }
        else if (const auto* const intValue = rtValue->as<const RuntimeIntegerValue*>())
        {
            return intValue->isSigned() ? static_cast<T>(intValue->getInt64()) : static_cast<T>(intValue->getUint64());
        }
        else if (const auto* const boolValue = rtValue->as<const RuntimeBooleanValue*>())
        {
            return static_cast<T>(boolValue->getBool() ? 1u : 0u);
        }
        else if (auto* const optValue = rtValue->as<RuntimeOptionalValue*>())
        {
            return !optValue->hasValue() ? static_cast<T>(0u) : runtimeValueCast<T>(optValue->getValue());
        }

        return NauMakeError("Can not convert to arithmetic type");
    }



}  // namespace nau
