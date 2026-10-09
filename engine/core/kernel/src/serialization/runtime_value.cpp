// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/serialization/runtime_value.h"

#include "nau/diag/error.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/serialization/serialization.h"
#include "nau/string/string_utils.h"

namespace nau
{

    namespace
    {
        class RuntimeValueRefImpl final : public RuntimeValueRef
        {
            NAU_CLASS_(nau::RuntimeValueRefImpl, RuntimeValueRef)

        public:
            RuntimeValueRefImpl(RuntimeValue::Ptr& valueRef) :
                m_valueRef(valueRef),
                m_isMutable(true)
            {
            }

            RuntimeValueRefImpl(const RuntimeValue::Ptr& valueRef) :
                m_valueRef(const_cast<RuntimeValue::Ptr&>(valueRef)),
                m_isMutable(true)
            {
            }

            bool isMutable() const override
            {
                return m_isMutable;
            }

            void set(RuntimeValue::Ptr value) override
            {
                m_valueRef = std::move(value);
            }

            RuntimeValue::Ptr get() const override
            {
                return m_valueRef;
            }

        private:
            RuntimeValue::Ptr& m_valueRef;
            const bool m_isMutable;
        };

        std::optional<std::string> lexicalCast(const RuntimePrimitiveValue& value)
        {
            if (auto iValue = value.as<const RuntimeIntegerValue*>())
            {
                return strings::lexicalCast(iValue->getInt64());
            }
            else if (auto fValue = value.as<const RuntimeFloatValue*>())
            {
                return strings::lexicalCast(fValue->getSingle());
            }
            else if (auto bValue = value.as<const RuntimeBooleanValue*>())
            {
                return strings::lexicalCast(bValue->getBool());
            }

            return std::nullopt;
        }

        bool lexicalCast(RuntimePrimitiveValue& value, const std::string& str)
        {
            if (auto iValue = value.as<RuntimeIntegerValue*>())
            {
                const int64_t v = str.empty() ? 0 : strings::lexicalCast<int64_t>(str);
                iValue->setInt64(v);
            }
            else if (auto fValue = value.as<RuntimeFloatValue*>())
            {
                const double v = str.empty() ? 0. : strings::lexicalCast<double>(str);
                fValue->setDouble(v);
            }
            else if (auto bValue = value.as<RuntimeBooleanValue*>())
            {
                const bool v = str.empty() ? false : strings::lexicalCast<bool>(str);
                bValue->setBool(v);
            }
            else
            {
                return false;
            }

            return true;
        }

        Result<> assignStringValue(RuntimeStringValue& dstStr, const RuntimePrimitiveValue& src, serialization::TypeCoercion typeCoercion)
        {
            if (auto const srcStr = src.as<const RuntimeStringValue*>())
            {
                auto strBytes = srcStr->getString();
                return dstStr.setString(strBytes);
            }

            const std::optional<std::string> str = typeCoercion == serialization::TypeCoercion::Allow ? lexicalCast(src) : std::nullopt;

            if (str)
            {
                return dstStr.setString(*str);
            }

            return NauMakeError("String value can be assigned only from other string");
        }

        Result<> assignPrimitiveValue(RuntimePrimitiveValue& dst, const RuntimePrimitiveValue& src, serialization::TypeCoercion typeCoercion = serialization::TypeCoercion::Allow)
        {
            using namespace nau::serialization;

            if (auto const dstStr = dst.as<RuntimeStringValue*>())
            {
                return assignStringValue(*dstStr, src, typeCoercion);
            }

            const auto tryImplicitCast = [&src, &dst, typeCoercion]
            {
                if (typeCoercion != TypeCoercion::Allow)
                {
                    return false;
                }

                auto const srcStr = src.as<const RuntimeStringValue*>();
                return srcStr && lexicalCast(dst, srcStr->getString());
            };

            if (auto const dstBool = dst.as<RuntimeBooleanValue*>())
            {
                if (auto const srcBool = src.as<const RuntimeBooleanValue*>())
                {
                    dstBool->setBool(srcBool->getBool());
                }
                else if (!tryImplicitCast())
                {
                    return NauMakeError("Fail to assign boolean value");
                }

                return {};
            }

            if (RuntimeFloatValue* const dstFloat = dst.as<RuntimeFloatValue*>())
            {
                if (auto const srcFloat = src.as<const RuntimeFloatValue*>())
                {
                    dstFloat->setDouble(srcFloat->getDouble());
                }
                else if (auto const srcInt = src.as<const RuntimeIntegerValue*>())
                {
                    if (dstFloat->getBitsCount() == sizeof(double))
                    {
                        dstFloat->setDouble(static_cast<double>(srcInt->get<int64_t>()));
                    }
                    else
                    {
                        dstFloat->setSingle(static_cast<float>(srcInt->get<int64_t>()));
                    }
                }
                else if (!tryImplicitCast())
                {
                    return NauMakeError("Can t assign value to float");
                }

                return {};
            }

            if (auto const dstInt = dst.as<RuntimeIntegerValue*>())
            {
                if (auto const srcInt = src.as<const RuntimeIntegerValue*>())
                {
                    if (dstInt->isSigned())
                    {
                        dstInt->setInt64(srcInt->getInt64());
                    }
                    else
                    {
                        dstInt->setUint64(srcInt->getUint64());
                    }
                }
                else if (auto const srcFloat = src.as<const RuntimeFloatValue*>())
                {
                    const auto iValue = static_cast<int64_t>(std::floor(srcFloat->getSingle()));

                    if (dstInt->isSigned())
                    {
                        dstInt->setInt64(iValue);
                    }
                    else
                    {
                        dstInt->setUint64(iValue);
                    }
                }
                else if (!tryImplicitCast())
                {
                    return NauMakeError("Can t assign value to integer");
                }

                return {};
            }

            return NauMakeError("Do not known how to assign primitive runtime value");
        }

        Result<> assignCollection(RuntimeCollection& dst, RuntimeReadonlyCollection& src, ValueAssignOptionFlag option)
        {
            if (!option.has(ValueAssignOption::MergeCollection))
            {
                dst.clear();
            }
            if (const size_t size = src.getSize(); size > 0)
            {
                dst.reserve(size);
                for (size_t i = 0; i < size; ++i)
                {
                    NauCheckResult(dst.append(src.getAt(i)));
                }
            }

            return {};
        }

        Result<> assignDictionary(RuntimeReadonlyDictionary& dst, RuntimeReadonlyDictionary& src, ValueAssignOptionFlag option)
        {
            if (RuntimeDictionary* const mutableDictionary = dst.as<RuntimeDictionary*>(); mutableDictionary && !option.has(ValueAssignOption::MergeCollection))
            {
                mutableDictionary->clear();
            }

            for (size_t i = 0, size = src.getSize(); i < size; ++i)
            {
                auto key = src.getKey(i);
                auto value = src.getValue(key);
                NauCheckResult(dst.setValue(key, value));
            }

            return {};
        }

        Result<> assignObject(RuntimeObject& obj, RuntimeReadonlyDictionary& srcDict)
        {
            for (size_t i = 0, size = obj.getSize(); i < size; ++i)
            {
                auto key = obj.getKey(i);
                if (auto srcValue = srcDict.getValue(key))
                {
                    NauCheckResult(obj.setFieldValue(key, srcValue));
                }
            }

            return {};
        }

    }  // namespace

    Result<> RuntimeValue::assign(RuntimeValue::Ptr dst, RuntimeValue::Ptr src, ValueAssignOptionFlag option)
    {
        NAU_ASSERT(dst);
        NAU_ASSERT(dst->isMutable());

        if (RuntimeValueRef* const valueRef = dst->as<RuntimeValueRef*>())
        {
            valueRef->set(std::move(src));
            return {};
        }

        NAU_ASSERT(src);

        const auto clearDstIfOptionalSrcIsNull = [&dst]() -> Result<>
        {
            if (auto const dstOpt = dst->as<RuntimeOptionalValue*>())
            {
                dstOpt->reset();
            }
            else if (auto const dstStr = dst->as<RuntimeStringValue*>())
            {
                NauCheckResult(dstStr->setString(""));
            }
            else if (auto const dstCollection = dst->as<RuntimeCollection*>())
            {
                dstCollection->clear();
            }
            else if (auto const dstDict = dst->as<RuntimeDictionary*>())
            {
                dstDict->clear();
            }

            return ResultSuccess;
        };

        if (RuntimeValueRef* const srcRefValue = src->as<RuntimeValueRef*>())
        {
            if (RuntimeValue::Ptr referencedValue = srcRefValue->get(); referencedValue)
            {
                return RuntimeValue::assign(dst, referencedValue);
            }

            return clearDstIfOptionalSrcIsNull();
        }

        if (RuntimeOptionalValue* const srcOpt = src->as<RuntimeOptionalValue*>())
        {
            if (srcOpt->hasValue())
            {
                return RuntimeValue::assign(dst, srcOpt->getValue());
            }

            return clearDstIfOptionalSrcIsNull();
        }

        if (auto const dstOpt = dst->as<RuntimeOptionalValue*>())
        {
            return dstOpt->setValue(src);
        }

        if (auto const dstValue = dst->as<RuntimePrimitiveValue*>())
        {
            const RuntimePrimitiveValue* const srcValue = src->as<RuntimePrimitiveValue*>();
            if (!srcValue)
            {
                return NauMakeError("Expected primitive value");
            }
            return assignPrimitiveValue(*dstValue, *srcValue);
        }

        if (auto const dstCollection = dst->as<RuntimeCollection*>())
        {
            RuntimeReadonlyCollection* const srcCollection = src->as<RuntimeReadonlyCollection*>();
            if (!srcCollection)
            {
                return NauMakeError("Expected Collection object");
            }

            return assignCollection(*dstCollection, *srcCollection, option);
        }

        if (auto* const dstCollection = dst->as<RuntimeReadonlyCollection*>(); dstCollection && src->is<RuntimeReadonlyCollection>())
        {
            RuntimeReadonlyCollection* const srcCollection = src->as<RuntimeReadonlyCollection*>();
            if (!srcCollection)
            {
                return NauMakeError("Expected Collection object");
            }

            const size_t srcSize = srcCollection->getSize();
            for (size_t i = 0, dstSize = dstCollection->getSize(); i < dstSize; ++i)
            {
                if (srcSize <= i)
                {
                    // check that dst is optional (and throw if not) ?
                    continue;
                }

                NauCheckResult(dstCollection->setAt(i, srcCollection->getAt(i)));
            }

            return {};
        }

        // must assign object prior dictionary:
        // each object is also dictionary, but logic is differ:  object has fixed set of the fields.
        if (auto* const dstObj = dst->as<RuntimeObject*>())
        {
            RuntimeReadonlyDictionary* const srcDict = src->as<RuntimeReadonlyDictionary*>();
            return srcDict ? assignObject(*dstObj, *srcDict) : NauMakeError("Expected Dictionary object");
        }

        if (auto* const dstDict = dst->as<RuntimeReadonlyDictionary*>())
        {
            RuntimeReadonlyDictionary* const srcDict = src->as<RuntimeReadonlyDictionary*>();
            return srcDict ? assignDictionary(*dstDict, *srcDict, option) : NauMakeError("Expected Dictionary object");
        }

        return NauMakeError("Do not known how to assign runtime value");
    }

    RuntimeValueRef::Ptr RuntimeValueRef::create(RuntimeValue::Ptr& value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<RuntimeValueRefImpl, RuntimeValueRef>(std::move(allocator), std::ref(value));
    }

    RuntimeValueRef::Ptr RuntimeValueRef::create(std::reference_wrapper<const RuntimeValue::Ptr> value, IMemAllocator::Ptr allocator)
    {
        return rtti::createInstanceWithAllocator<RuntimeValueRefImpl, RuntimeValueRef>(std::move(allocator), value);
    }

}  // namespace nau
