// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <type_traits>

#include "nau/diag/assertion.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/serialization/native_runtime_value/native_value_base.h"

namespace nau::ser_detail
{

    /**
     */
    template <typename T>
    class NativeIntegerValue final : public ser_detail::NativePrimitiveRuntimeValueBase<RuntimeIntegerValue>
    {
        /**
            template <typename T> -> template<std::integral T>
            must not be used (because T can be reference/const reference, so std::integral<T> will fail).
        */

        using Base = ser_detail::NativePrimitiveRuntimeValueBase<RuntimeIntegerValue>;
        using IntegralValueType = std::remove_const_t<std::remove_reference_t<T>>;

        NAU_CLASS_(NativeIntegerValue<T>, Base)

    public:
        static inline constexpr bool IsMutable = !std::is_const_v<std::remove_reference_t<T>>;

        NativeIntegerValue(T value) :
            m_value(value)
        {
        }

        bool isMutable() const override
        {
            return IsMutable;
        }

        bool isSigned() const override
        {
            return std::is_signed_v<IntegralValueType>;
        }

        size_t getBitsCount() const override
        {
            return sizeof(IntegralValueType);
        }

        void setInt64([[maybe_unused]] int64_t value) override
        {
            if constexpr(IsMutable)
            {
                value_changes_scope;
                m_value = static_cast<IntegralValueType>(value);
            }
            else
            {
                NAU_FAILURE("Attempt to modify non mutable value");
            }
        }

        void setUint64([[maybe_unused]] uint64_t value) override
        {
            if constexpr(IsMutable)
            {
                value_changes_scope;
                m_value = static_cast<IntegralValueType>(value);
            }
            else
            {
                NAU_FAILURE_ALWAYS("Attempt to modify non mutable value");
            }
        }

        int64_t getInt64() const override
        {
            return static_cast<int64_t>(m_value);
        }

        uint64_t getUint64() const override
        {
            return static_cast<uint64_t>(m_value);
        }

    private:
        T m_value;
    };

}  // namespace nau::ser_detail

namespace nau
{
    template <std::integral T>
    RuntimeIntegerValue::Ptr makeValueRef(T& value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<ser_detail::NativeIntegerValue<T&>>(std::move(allocator), value);
    }

    template <std::integral T>
    RuntimeIntegerValue::Ptr makeValueRef(const T& value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<ser_detail::NativeIntegerValue<const T&>>(std::move(allocator), value);
    }

    template <std::integral T>
    RuntimeIntegerValue::Ptr makeValueCopy(T value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<ser_detail::NativeIntegerValue<T>>(std::move(allocator), value);
    }
}  // namespace nau
