// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/serialization/native_runtime_value/type_info_value.h"

#include "nau/rtti/rtti_impl.h"
#include "nau/serialization/native_runtime_value/native_value_base.h"

namespace nau::rtti
{
    class RuntimeTypeInfoValueImpl final : public ser_detail::NativePrimitiveRuntimeValueBase<RuntimeTypeInfoValue>,
                                           public virtual RuntimeIntegerValue
    {
        using Base = ser_detail::NativePrimitiveRuntimeValueBase<RuntimeTypeInfoValue>;
        NAU_CLASS_(nau::rtti::RuntimeTypeInfoValueImpl, Base, RuntimeIntegerValue)

        RuntimeTypeInfoValueImpl(const RuntimeTypeInfoValueImpl&) = delete;

    public:
        RuntimeTypeInfoValueImpl() :
            m_isMutable(true),
            m_typeInfoHolder(makeTypeInfoFromId(0)),
            m_typeInfoPtr(&m_typeInfoHolder)
        {
        }

        RuntimeTypeInfoValueImpl(TypeInfo typeInfoRef) :
            m_isMutable(true),
            m_typeInfoHolder(typeInfoRef),
            m_typeInfoPtr(&m_typeInfoHolder)
        {
        }

        explicit RuntimeTypeInfoValueImpl(std::reference_wrapper<TypeInfo> typeInfoRef) :
            m_isMutable(true),
            m_typeInfoHolder(makeTypeInfoFromId(0)),
            m_typeInfoPtr(&typeInfoRef.get())
        {
        }

        explicit RuntimeTypeInfoValueImpl(std::reference_wrapper<const TypeInfo> typeInfoRef) :
            m_isMutable(false),
            m_typeInfoHolder(makeTypeInfoFromId(0)),
            m_typeInfoPtr(const_cast<rtti::TypeInfo*>(&typeInfoRef.get()))

        {
        }

        bool isMutable() const override
        {
            return m_isMutable;
        }

        bool isSigned() const override
        {
            return false;
        }

        size_t getBitsCount() const override
        {
            return sizeof(size_t);
        }

        void setInt64(int64_t value) override
        {
            NAU_ASSERT(m_isMutable, "Attempt to modify non mutable value");
            NAU_FATAL(m_typeInfoPtr);

            if (m_isMutable)
            {
                value_changes_scope;
                *m_typeInfoPtr = makeTypeInfoFromId(static_cast<size_t>(value));
            }
        }

        void setUint64(uint64_t value) override
        {
            NAU_ASSERT(m_isMutable, "Attempt to modify non mutable value");
            NAU_FATAL(m_typeInfoPtr);

            if (m_isMutable)
            {
                value_changes_scope;
                *m_typeInfoPtr = makeTypeInfoFromId(static_cast<size_t>(value));
            }
        }

        int64_t getInt64() const override
        {
            NAU_FATAL(m_typeInfoPtr);

            const auto typeId = static_cast<uint64_t>(m_typeInfoPtr->getHashCode());
            NAU_ASSERT(typeId <= std::numeric_limits<int64_t>::max());

            const int64_t t = static_cast<int64_t>(typeId);
            return t;
        }

        uint64_t getUint64() const override
        {
            NAU_FATAL(m_typeInfoPtr);
            return static_cast<uint64_t>(m_typeInfoPtr->getHashCode());
        }

        const TypeInfo& getTypeInfo() const override
        {
            NAU_FATAL(m_typeInfoPtr);
            return *m_typeInfoPtr;
        }

        void setTypeInfo(const TypeInfo& type) override
        {
            NAU_FATAL(m_typeInfoPtr);
            value_changes_scope;
            *m_typeInfoPtr = type;
        }

        const bool m_isMutable;
        TypeInfo m_typeInfoHolder;
        TypeInfo* const m_typeInfoPtr = nullptr;
    };

    RuntimeValue::Ptr makeValueRef(TypeInfo& value, [[maybe_unused]] IMemAllocator::Ptr allocator)
    {
        return rtti::createInstance<RuntimeTypeInfoValueImpl>(std::ref(value));
    }

    RuntimeValue::Ptr makeValueRef(const TypeInfo& value, [[maybe_unused]] IMemAllocator::Ptr allocator)
    {
        return rtti::createInstance<RuntimeTypeInfoValueImpl>(std::cref(value));
    }

    RuntimeValue::Ptr makeValueCopy(TypeInfo value, [[maybe_unused]] IMemAllocator::Ptr allocator)
    {
        return rtti::createInstance<RuntimeTypeInfoValueImpl>(value);
    }
}  // namespace nau::rtti