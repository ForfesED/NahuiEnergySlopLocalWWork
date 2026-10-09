// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <concepts>
#include <type_traits>

#include "nau/diag/assertion.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/serialization/native_runtime_value/native_value_base.h"

namespace nau
{
    /**
     */
    template <typename T>
    class NativeBooleanValue final : public ser_detail::NativePrimitiveRuntimeValueBase<RuntimeBooleanValue>
    {
        using Base = ser_detail::NativePrimitiveRuntimeValueBase<RuntimeBooleanValue>;

        NAU_CLASS_(NativeBooleanValue<T>, Base)

    public:
        static inline constexpr bool IsMutable = !std::is_const_v<std::remove_reference_t<T>>;

        NativeBooleanValue(T value) :
            m_value(value)
        {
        }

        bool isMutable() const override
        {
            return IsMutable;
        }

        void setBool(bool value) override
        {
            if constexpr(IsMutable)
            {
                value_changes_scope;
                m_value = value;
            }
            else
            {
                NAU_FAILURE("Attempt to modify non mutable value");
            }
        }

        bool getBool() const override
        {
            return m_value;
        }

    private:
        T m_value;
    };

    /**
    */
    inline RuntimeBooleanValue::Ptr makeValueRef(bool& value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<NativeBooleanValue<bool&>>(std::move(allocator), value);
    }

    inline RuntimeBooleanValue::Ptr makeValueRef(const bool& value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<NativeBooleanValue<const bool&>>(std::move(allocator), value);
    }

    inline RuntimeBooleanValue::Ptr makeValueCopy(bool value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<NativeBooleanValue<bool>>(std::move(allocator), value);
    }

}  // namespace nau
