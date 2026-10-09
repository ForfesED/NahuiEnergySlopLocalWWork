// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <type_traits>

#include "nau/rtti/rtti_impl.h"
#include "nau/serialization/native_runtime_value/native_value_base.h"
#include "nau/serialization/native_runtime_value/native_value_forwards.h"

namespace nau::ser_detail
{
    /**
     */
    template <typename T>
    class NativeTuple final : public ser_detail::NativeRuntimeValueBase<RuntimeReadonlyCollection>
    {
        using Base = ser_detail::NativeRuntimeValueBase<RuntimeReadonlyCollection>;
        using TupleType = std::decay_t<T>;

        NAU_CLASS_(NativeTuple<T>, Base)

    public:
        static_assert(LikeTuple<TupleType>);
        static inline constexpr bool IsMutable = !std::is_const_v<std::remove_reference_t<T>>;
        static inline constexpr bool IsReference = std::is_lvalue_reference_v<T>;
        static inline constexpr size_t TupleSize = TupleValueOperations1<TupleType>::TupleSize;

        NativeTuple(T inTuple)
        requires(IsReference)
            :
            m_tuple(inTuple)
        {
        }

        template <typename U>
        NativeTuple(U&& inTuple)
        requires(!IsReference)
            :
            m_tuple(std::forward<U>(inTuple))
        {
        }

        bool isMutable() const override
        {
            return IsMutable;
        }

        size_t getSize() const override
        {
            return TupleSize;
        }

        RuntimeValue::Ptr getAt(size_t index) override
        {
            return getElementInternal(index,  std::make_index_sequence<TupleSize>{});
        }

        Result<> setAt(size_t index, const RuntimeValue::Ptr& value) override
        {
            NAU_ASSERT(value);
            return RuntimeValue::assign(getAt(index), value);
        }

    private:
        template <size_t... I>
        RuntimeValue::Ptr getElementInternal(size_t index, std::index_sequence<I...>) const
        {
            NAU_ASSERT(index < TupleSize, "Bad element index ({})", index);

            using ElementAccessorFunc = RuntimeValue::Ptr (*)(const NativeTuple<T>& self, std::add_lvalue_reference_t<T>);

            const std::array<ElementAccessorFunc, TupleSize> factories{wrapElementAsRuntimeValue<I>...};

            auto& f = factories[index];
            return f(*this, const_cast<TupleType&>(m_tuple));
        }

        template <size_t I>
        static RuntimeValue::Ptr wrapElementAsRuntimeValue(const NativeTuple<T>& self, std::add_lvalue_reference_t<T> container)
        {
            decltype(auto) el = TupleValueOperations1<TupleType>::template element<I>(container);
            return self.makeChildValue(makeValueRef(el));
        }

        T m_tuple;
    };

    template <typename T>
    class NativeUniformTuple final : public ser_detail::NativeRuntimeValueBase<RuntimeReadonlyCollection>
    {
        using Base = ser_detail::NativeRuntimeValueBase<RuntimeReadonlyCollection>;
        using TupleType = std::decay_t<T>;

        NAU_CLASS_(NativeUniformTuple<T>, Base)

    public:
        static_assert(LikeUniformTuple<TupleType>);
        static inline constexpr bool IsMutable = !std::is_const_v<std::remove_reference_t<T>>;
        static inline constexpr bool IsReference = std::is_lvalue_reference_v<T>;
        static inline constexpr size_t TupleSize = UniformTupleValueOperations<TupleType>::TupleSize;

        NativeUniformTuple(T inTuple)
        requires(IsReference)
            :
            m_tuple(inTuple)
        {
        }

        template <typename U>
        NativeUniformTuple(U&& inTuple)
        requires(!IsReference)
            :
            m_tuple(std::forward<U>(inTuple))
        {
        }

        bool isMutable() const override
        {
            return IsMutable;
        }

        size_t getSize() const override
        {
            return TupleSize;
        }

        RuntimeValue::Ptr getAt(size_t index) override
        {
            decltype(auto) el = UniformTupleValueOperations<TupleType>::element(m_tuple, index);

            return this->makeChildValue(makeValueRef(el));
        }

        Result<> setAt(size_t index, const RuntimeValue::Ptr& value) override
        {
            NAU_ASSERT(value);
            NAU_ASSERT(index < TupleSize);

            decltype(auto) el = UniformTupleValueOperations<TupleType>::element(m_tuple, index);

            return RuntimeValue::assign(makeValueRef(el), value);
        }

    private:
        T m_tuple;
    };

}  // namespace nau::ser_detail

namespace nau
{
    template <LikeTuple T>
    RuntimeReadonlyCollection::Ptr makeValueRef(T& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeTuple<T&>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), tup);
    }

    template <LikeTuple T>
    RuntimeReadonlyCollection::Ptr makeValueRef(const T& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeTuple<const T&>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), tup);
    }

    template <LikeTuple T>
    RuntimeReadonlyCollection::Ptr makeValueCopy(const T& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeTuple<T>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), tup);
    }

    template <LikeTuple T>
    RuntimeReadonlyCollection::Ptr makeValueCopy(T&& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeTuple<T>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), std::move(tup));
    }

    template <LikeUniformTuple T>
    RuntimeReadonlyCollection::Ptr makeValueRef(T& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeUniformTuple<T&>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), tup);
    }

    template <LikeUniformTuple T>
    RuntimeReadonlyCollection::Ptr makeValueRef(const T& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeUniformTuple<const T&>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), tup);
    }

    template <LikeUniformTuple T>
    RuntimeReadonlyCollection::Ptr makeValueCopy(const T& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeUniformTuple<T>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), tup);
    }

    template <LikeUniformTuple T>
    RuntimeReadonlyCollection::Ptr makeValueCopy(T&& tup, IMemAllocator::Ptr allocator)
    {
        using Tuple = ser_detail::NativeUniformTuple<T>;
        return rtti::createInstanceWithAllocator<Tuple>(std::move(allocator), std::move(tup));
    }
}  // namespace nau
