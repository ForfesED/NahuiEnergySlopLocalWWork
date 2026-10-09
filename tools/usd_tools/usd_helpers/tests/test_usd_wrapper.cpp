// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <gmock/gmock.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/usd/usdGeom/xform.h>

#include "nau/usd_wrapper/usd_attribute_wrapper.h"
#include "nau/usd_wrapper/usd_runtime_vector_value.h"

namespace nau::test
{
    using namespace ::testing;

    static constexpr int64_t g_testInt = -5;
    static constexpr uint64_t g_testUnsignedInt = 5;
    static constexpr float g_testFloat = 45.0f;
    static constexpr double g_testDouble = 45.0;
    static const std::string g_testString = "test_string";

    namespace
    {
        struct TestStage
        {
            PXR_NS::UsdStageRefPtr stage;
            PXR_NS::UsdPrim prim;
            PXR_NS::UsdAttribute attribute;

            TestStage()
            {
                stage = PXR_NS::UsdStage::CreateInMemory("TestStage");
                prim = stage->DefinePrim(PXR_NS::SdfPath("/TestPrim"), PXR_NS::TfToken("TestPrim"));
            }

            template <typename Type, typename ValueType>
            PXR_NS::UsdAttribute createAttribute(Type type, ValueType value)
            {
                attribute = prim.CreateAttribute(PXR_NS::TfToken("testAttribute"), type);
                attribute.Set(value);
                return attribute;
            }
        };
    }  // namespace

    TEST(TestAttributeWrapper, GetBooleanValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Bool, true);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeBooleanValue*>();

        PXR_NS::VtValue value;
        attribute.Get(&value);
        bool attributeValue = value.Get<bool>();

        EXPECT_EQ(runtimeValue->getBool(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetBooleanToRuntimeValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Bool, true);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeBooleanValue*>();
        runtimeValue->setBool(false);

        PXR_NS::VtValue value;
        attribute.Get(&value);
        bool attributeValue = value.Get<bool>();

        EXPECT_EQ(runtimeValue->getBool(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetBooleanToAttribute)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Bool, true);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeBooleanValue*>();

        attribute.Set(false);

        PXR_NS::VtValue value;
        attribute.Get(&value);
        bool attributeValue = value.Get<bool>();

        EXPECT_EQ(runtimeValue->getBool(), attributeValue);
    }

    TEST(TestAttributeWrapper, GetStringValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->String, g_testString);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeStringValue*>();

        PXR_NS::VtValue value;
        attribute.Get(&value);
        std::string attributeValue = value.Get<std::string>();

        EXPECT_EQ(runtimeValue->getString(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetStringToRuntimeValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->String, g_testString);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeStringValue*>();
        runtimeValue->setString("another_test_string").ignore();

        PXR_NS::VtValue value;
        attribute.Get(&value);
        std::string attributeValue = value.Get<std::string>();

        EXPECT_EQ(runtimeValue->getString(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetStringToAttribute)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->String, g_testString);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeStringValue*>();

        attribute.Set("another_test_string");

        PXR_NS::VtValue value;
        attribute.Get(&value);
        std::string attributeValue = value.Get<std::string>();

        EXPECT_EQ(runtimeValue->getString(), attributeValue);
    }

    TEST(TestAttributeWrapper, GetFloatValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Float, g_testFloat);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeFloatValue*>();

        PXR_NS::VtValue value;
        attribute.Get(&value);
        float attributeValue = value.Get<float>();

        EXPECT_EQ(runtimeValue->getSingle(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetFloatToRuntimeValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Float, g_testFloat);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeFloatValue*>();
        runtimeValue->setSingle(90.0f);

        PXR_NS::VtValue value;
        attribute.Get(&value);
        float attributeValue = value.Get<float>();

        EXPECT_EQ(runtimeValue->getSingle(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetFloatToAttribute)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Float, g_testFloat);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeFloatValue*>();

        attribute.Set(90.0f);

        PXR_NS::VtValue value;
        attribute.Get(&value);
        float attributeValue = value.Get<float>();

        EXPECT_EQ(runtimeValue->getSingle(), attributeValue);
    }

    TEST(TestAttributeWrapper, GetDoubleValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Double, g_testDouble);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeFloatValue*>();

        PXR_NS::VtValue value;
        attribute.Get(&value);
        double attributeValue = value.Get<double>();

        EXPECT_EQ(runtimeValue->getDouble(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetDoubleToRuntimeValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Double, g_testDouble);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeFloatValue*>();
        runtimeValue->setDouble(90.0);

        PXR_NS::VtValue value;
        attribute.Get(&value);
        double attributeValue = value.Get<double>();

        EXPECT_EQ(runtimeValue->getDouble(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetDoubleToAttribute)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Double, g_testDouble);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeFloatValue*>();

        attribute.Set(90.0);

        PXR_NS::VtValue value;
        attribute.Get(&value);
        double attributeValue = value.Get<double>();

        EXPECT_EQ(runtimeValue->getDouble(), attributeValue);
    }

    TEST(TestAttributeWrapper, GetInt64Value)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Int64, g_testInt);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeIntegerValue*>();

        PXR_NS::VtValue value;
        attribute.Get(&value);
        int64_t attributeValue = value.Get<int64_t>();

        EXPECT_EQ(runtimeValue->getInt64(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetInt64ToRuntimeValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Int64, g_testInt);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeIntegerValue*>();
        runtimeValue->setInt64(int64_t(2));

        PXR_NS::VtValue value;
        attribute.Get(&value);
        int64_t attributeValue = value.Get<int64_t>();

        EXPECT_EQ(runtimeValue->getInt64(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetInt64ToAttribute)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->Int64, g_testInt);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeIntegerValue*>();

        attribute.Set(int64_t(2));

        PXR_NS::VtValue value;
        attribute.Get(&value);
        int64_t attributeValue = value.Get<int64_t>();

        EXPECT_EQ(runtimeValue->getInt64(), attributeValue);
    }

    TEST(TestAttributeWrapper, GetUnsignedInt64Value)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->UInt64, g_testUnsignedInt);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeIntegerValue*>();

        PXR_NS::VtValue value;
        attribute.Get(&value);
        uint64_t attributeValue = value.Get<uint64_t>();

        EXPECT_EQ(runtimeValue->getUint64(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetUnsignedInt64ToRuntimeValue)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->UInt64, g_testUnsignedInt);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeIntegerValue*>();
        runtimeValue->setUint64(uint64_t(2));

        PXR_NS::VtValue value;
        attribute.Get(&value);
        uint64_t attributeValue = value.Get<uint64_t>();

        EXPECT_EQ(runtimeValue->getUint64(), attributeValue);
    }

    TEST(TestAttributeWrapper, SetUnsignedInt64ToAttribute)
    {
        TestStage testStage;
        PXR_NS::UsdAttribute attribute = testStage.createAttribute(PXR_NS::SdfValueTypeNames->UInt64, g_testUnsignedInt);

        auto runtimeValue = nau::attributeAsRuntimeValue(attribute)->as<nau::RuntimeIntegerValue*>();

        attribute.Set(uint64_t(2));

        PXR_NS::VtValue value;
        attribute.Get(&value);
        uint64_t attributeValue = value.Get<uint64_t>();

        EXPECT_EQ(runtimeValue->getUint64(), attributeValue);
    }
}  // namespace nau::test
