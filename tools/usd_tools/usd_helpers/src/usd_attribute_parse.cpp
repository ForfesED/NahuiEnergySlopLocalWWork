// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include <pxr/base/gf/vec3f.h>

#include <string>

#include "nau/usd_wrapper/usd_attribute_wrapper.h"
#include "nau/usd_wrapper/usd_runtime_vector_value.h"

namespace nau
{
    namespace
    {
        class AttributeRuntimeIntegerValue final : public RuntimeIntegerValue
        {
            NAU_CLASS(nau::AttributeRuntimeIntegerValue, rtti::RCPolicy::Concurrent, RuntimeIntegerValue)

        public:
            AttributeRuntimeIntegerValue(PXR_NS::UsdAttribute& attribute) :
                m_attribute(attribute)
            {
            }

            bool isMutable() const override
            {
                return false;
            }

            bool isSigned() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                return !(value.IsHolding<uint64_t>() || value.IsHolding<uint32_t>());
            }

            size_t getBitsCount() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                return (value.IsHolding<int32_t>() || value.IsHolding<uint32_t>() ? sizeof(int32_t) : sizeof(int64_t));
            }

            void setInt64(int64_t value) override
            {
                m_attribute.Set(value);
            }

            void setUint64(uint64_t value) override
            {
                m_attribute.Set(value);
            }

            int64_t getInt64() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                NAU_ASSERT(value.IsHolding<int64_t>());
                return value.Get<int64_t>();
            }

            uint64_t getUint64() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                NAU_ASSERT(value.IsHolding<uint64_t>());
                return value.Get<uint64_t>();
            }

        private:
            PXR_NS::UsdAttribute& m_attribute;
        };

        class AttributeRuntimeFloatValue final : public RuntimeFloatValue
        {
            NAU_CLASS(nau::AttributeRuntimeFloatValue, rtti::RCPolicy::Concurrent, RuntimeFloatValue)

        public:
            AttributeRuntimeFloatValue(PXR_NS::UsdAttribute& attribute) :
                m_attribute(std::ref(attribute))
            {
            }

            bool isMutable() const override
            {
                return false;
            }

            size_t getBitsCount() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                return value.IsHolding<double>() ? sizeof(double) : sizeof(float);
            }

            void setDouble(double value) override
            {
                m_attribute.Set(value);
            }

            void setSingle(float value) override
            {
                m_attribute.Set(value);
            }

            double getDouble() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);

                // NOTE: The implementation specifics of copying a runtime value always expects double precision.
                double result;
                if (value.IsHolding<float>())
                {
                    result = value.Get<float>();
                }
                else if (value.IsHolding<double>())
                {
                    result = value.Get<double>();
                }
                else
                {
                    NAU_ASSERT("Unexpected holding type value");
                }

                return result;
            }

            float getSingle() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                NAU_ASSERT(value.IsHolding<float>());
                return value.Get<float>();
            }

        private:
            PXR_NS::UsdAttribute& m_attribute;
        };

        class AttributeRuntimeStringValue final : public RuntimeStringValue
        {
            NAU_CLASS(nau::AttributeRuntimeStringValue, rtti::RCPolicy::Concurrent, RuntimeStringValue)

        public:
            AttributeRuntimeStringValue(PXR_NS::UsdAttribute& attribute) :
                m_attribute(std::ref(attribute))
            {
            }

            bool isMutable() const override
            {
                return false;
            }

            Result<> setString(std::string_view value) override
            {
                m_attribute.Set(std::string(value));
                return ResultSuccess;
            }

            std::string getString() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                NAU_ASSERT(value.IsHolding<std::string>());
                return value.Get<std::string>();
            }

        private:
            PXR_NS::UsdAttribute& m_attribute;
        };

        class AttributeRuntimeBooleanValue final : public RuntimeBooleanValue
        {
            NAU_CLASS(nau::AttributeRuntimeBooleanValue, rtti::RCPolicy::Concurrent, RuntimeBooleanValue)

        public:
            AttributeRuntimeBooleanValue(PXR_NS::UsdAttribute& attribute) :
                m_attribute(std::ref(attribute))
            {
            }

            bool isMutable() const override
            {
                return false;
            }

            void setBool(bool value) override
            {
                m_attribute.Set(value);
            }

            bool getBool() const override
            {
                PXR_NS::VtValue value;
                m_attribute.Get(&value);
                NAU_ASSERT(value.IsHolding<bool>());
                return value.Get<bool>();
            }

        private:
            PXR_NS::UsdAttribute& m_attribute;
        };

        RuntimeValue::Ptr createAttributeRuntimeValue(PXR_NS::UsdAttribute& attribute)
        {
            PXR_NS::VtValue value;
            attribute.Get(&value);
            if (value.IsHolding<bool>())
            {
                return rtti::createInstance<AttributeRuntimeBooleanValue>(attribute);
            }
            else if (value.IsHolding<int32_t>() || value.IsHolding<uint32_t>() || value.IsHolding<int64_t>() || value.IsHolding<uint64_t>())
            {
                return rtti::createInstance<AttributeRuntimeIntegerValue>(attribute);
            }
            else if (value.IsHolding<std::string>())
            {
                return rtti::createInstance<AttributeRuntimeStringValue>(attribute);
            }
            else if (value.IsHolding<double>() || value.IsHolding<float>())
            {
                return rtti::createInstance<AttributeRuntimeFloatValue>(attribute);
            }
            else if (value.IsHolding<PXR_NS::GfVec3f>())
            {
                using VecValue = VecXAttributeRuntimeValue<PXR_NS::GfVec3f, 3>;
                return rtti::createInstance<VecValue>(attribute);
            }
            else if (value.IsHolding<PXR_NS::GfVec3d>())
            {
                using VecValue = VecXAttributeRuntimeValue<PXR_NS::GfVec3d, 3>;
                return rtti::createInstance<VecValue>(attribute);
            }

            NAU_LOG_WARNING("Unhandled attribute value type while creating runtime value");
            return nullptr;
        }
    }  // namespace

    RuntimeValue::Ptr attributeAsRuntimeValue(PXR_NS::UsdAttribute& attribute)
    {
        return createAttributeRuntimeValue(attribute);
    }
}  // namespace nau
