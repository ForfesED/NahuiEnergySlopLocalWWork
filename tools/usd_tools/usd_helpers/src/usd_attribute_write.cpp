// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/usd_wrapper/usd_attribute_wrapper.h"

namespace nau
{
    namespace
    {
        void makeAttributePrimitiveValue(PXR_NS::UsdAttribute& attribute, const RuntimePrimitiveValue& value)
        {
            if (auto integer = value.as<const RuntimeIntegerValue*>(); integer)
            {
                if (integer->isSigned())
                {
                    attribute.Set(integer->getInt64());
                }
                else
                {
                    attribute.Set(integer->getUint64());
                }
            }
            else if (auto floatPoint = value.as<const RuntimeFloatValue*>(); floatPoint)
            {
                if (floatPoint->getBitsCount() == sizeof(double))
                {
                    attribute.Set(floatPoint->getDouble());
                }
                else
                {
                    attribute.Set(floatPoint->getSingle());
                }
            }
            else if (auto str = value.as<const RuntimeStringValue*>(); str)
            {
                auto text = str->getString();
                attribute.Set(text);
            }
            else if (auto boolValue = value.as<const RuntimeBooleanValue*>(); boolValue)
            {
                attribute.Set(boolValue->getBool());
            }
            else
            {
                NAU_FAILURE("Unknown primitive type for attribute serialization");
            }
        }

        Result<> makeAttributeValue(PXR_NS::UsdAttribute& attribute, const RuntimeValue::Ptr& value)
        {
            if (const RuntimePrimitiveValue* const primitiveValue = value->as<const RuntimePrimitiveValue*>(); primitiveValue)
            {
                makeAttributePrimitiveValue(attribute, *primitiveValue);
            }
            else
            {
                NAU_FAILURE("Unhandled runtime value type for attribute serialization");
            }
            return ResultSuccess;
        }

    }  // namespace

    Result<> runtimeApplyToAttributeValue(PXR_NS::UsdAttribute& attribute, const RuntimeValue::Ptr& runtimeValue)
    {
        return makeAttributeValue(attribute, runtimeValue);
    }

    Result<> createAttributeByValue(PXR_NS::UsdPrim prim, PXR_NS::TfToken attributeName, const RuntimeValue::Ptr& runtimeValue)
    {
        auto editTarget = prim.GetStage()->GetEditTarget();
        auto targetPath = editTarget.MapToSpecPath(prim.GetPath());
        auto primSpec = targetPath.IsEmpty() ? PXR_NS::SdfPrimSpecHandle() : PXR_NS::SdfCreatePrimInLayer(editTarget.GetLayer(), targetPath);
        auto attribute = primSpec->GetAttributeAtPath(targetPath.AppendProperty(attributeName));
        if (const RuntimePrimitiveValue* const primitiveValue = runtimeValue->as<const RuntimePrimitiveValue*>(); !attribute && primitiveValue)
        {
            if (auto integer = (*primitiveValue).as<const RuntimeIntegerValue*>())
            {
                attribute = PXR_NS::SdfAttributeSpec::New(primSpec, attributeName.GetString(), PXR_NS::SdfValueTypeNames->Int);
            }
            else if (auto floatPoint = (*primitiveValue).as<const RuntimeFloatValue*>(); floatPoint)
            {
                if (floatPoint->getBitsCount() == sizeof(double))
                {
                    attribute = PXR_NS::SdfAttributeSpec::New(primSpec, attributeName.GetString(), PXR_NS::SdfValueTypeNames->Double);
                }
                else
                {
                    attribute = PXR_NS::SdfAttributeSpec::New(primSpec, attributeName.GetString(), PXR_NS::SdfValueTypeNames->Float);
                }
            }
            else if (auto str = (*primitiveValue).as<const RuntimeStringValue*>())
            {
                attribute = PXR_NS::SdfAttributeSpec::New(primSpec, attributeName.GetString(), PXR_NS::SdfValueTypeNames->String);
            }
            else if (auto boolValue = (*primitiveValue).as<const RuntimeBooleanValue*>())
            {
                attribute = PXR_NS::SdfAttributeSpec::New(primSpec, attributeName.GetString(), PXR_NS::SdfValueTypeNames->Bool);
            }
            else
            {
                NAU_FAILURE("Unknown primitive type for attribute serialization");
            }
        }
        else
        {
            return NauMakeError("Unhandled runtime value type for attribute serialization");
        }

        auto primAttribute = prim.GetAttribute(attributeName);
        return runtimeApplyToAttributeValue(primAttribute, runtimeValue);
    }
}  // namespace nau
