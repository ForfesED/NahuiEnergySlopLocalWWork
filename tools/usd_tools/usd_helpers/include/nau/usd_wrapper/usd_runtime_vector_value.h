// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/serialization/runtime_value_builder.h"

namespace nau
{
    template <typename T, size_t Size>
    class VecXAttributeRuntimeValue : public ser_detail::NativePrimitiveRuntimeValueBase<RuntimeReadonlyCollection>,
                                      public RuntimeReadonlyDictionary
    {
        using BasePrimitiveValue = ser_detail::NativePrimitiveRuntimeValueBase<RuntimeReadonlyCollection>;
        using ThisType = VecXAttributeRuntimeValue<T, Size>;

        NAU_CLASS_(ThisType, BasePrimitiveValue, RuntimeReadonlyDictionary)

    public:
        VecXAttributeRuntimeValue(PXR_NS::UsdAttribute& attribute) :
            m_attribute(attribute)
        {
        }

        bool isMutable() const override
        {
            return true;
        }

        size_t getSize() const override
        {
            return getFieldNames().size();
        }

        RuntimeValue::Ptr getAt(size_t index) override
        {
            NAU_ASSERT(index < getSize());
            PXR_NS::VtValue attributeValue;
            m_attribute.Get(&attributeValue);
            T vec = attributeValue.Get<T>();
            return makeValueCopy(vec[index]);
        }

        Result<> setAt(size_t index, const RuntimeValue::Ptr& value) override
        {
            NAU_ASSERT(index < getSize());

            auto castResult = runtimeValueCast<float>(value);
            NauCheckResult(castResult)

            value_changes_scope;
            PXR_NS::VtValue attributeValue;
            m_attribute.Get(&attributeValue);
            T vec = attributeValue.Get<T>();
            vec[static_cast<size_t>(index)] = *castResult;
            m_attribute.Set(vec);
            return ResultSuccess;
        }

        std::string_view getKey(size_t index) const override
        {
            NAU_ASSERT(index < getSize());

            return getFieldNames()[index];
        }

        RuntimeValue::Ptr getValue(std::string_view key) override
        {
            if (const auto index = getElementIndex(key))
            {
                NAU_ASSERT(index < getSize());
                PXR_NS::VtValue attributeValue;
                m_attribute.Get(&attributeValue);
                T vec = attributeValue.Get<T>();
                return makeValueCopy(vec[*index]);
            }

            return nullptr;
        }

        Result<> setValue(std::string_view key, const RuntimeValue::Ptr& value) override
        {
            if (const auto index = getElementIndex(key))
            {
                value_changes_scope;

                const float elem = *runtimeValueCast<float>(value);
                PXR_NS::VtValue attributeValue;
                m_attribute.Get(&attributeValue);
                T vec = attributeValue.Get<T>();
                vec[static_cast<size_t>(*index)] = elem;
                m_attribute.Set(vec);

                return ResultSuccess;
            }

            return NauMakeError("Unknown vec elem ({})", key);
        }

        bool containsKey(std::string_view key) const override
        {
            const auto keys = getFieldNames();

            return std::any_of(keys.begin(), keys.end(), [key](const char* fieldName)
            {
                return strings::icaseEqual(key, fieldName);
            });
        }

    private:
        static consteval auto getFieldNames()
        {
            static_assert(2 <= Size && Size <= 4);

            if constexpr (Size == 2)
            {
                return std::array<const char*, Size>{"x", "y"};
            }
            else if constexpr (Size == 3)
            {
                return std::array<const char*, Size>{"x", "y", "z"};
            }
            else
            {
                return std::array<const char*, Size>{"x", "y", "z", "w"};
            }
        }

        static inline std::optional<size_t> getElementIndex(std::string_view key)
        {
            const auto keys = getFieldNames();
            auto index = std::find_if(keys.begin(), keys.end(), [key](const char* fieldName)
            {
                return strings::icaseEqual(key, fieldName);
            });

            if (index == keys.end())
            {
                NAU_FAILURE("Invalid field ({})", key);
                return std::nullopt;
            }

            return static_cast<size_t>(index - keys.begin());
        }

        PXR_NS::UsdAttribute& m_attribute;
    };
}  // namespace nau
