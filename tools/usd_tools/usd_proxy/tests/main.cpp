// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <gmock/gmock.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/base/plug/registry.h>
#include <pxr/base/plug/plugin.h>

#include "usd_proxy/usd_proxy.h"

#include <stdlib.h> 

namespace Naueditor::test
{
    class TestDecorator : public UsdProxy::IUsdProxyPrimDecorator
    {

    public:
        void decorate(UsdProxy::ProxyPrimContextPtr context) override
        {
            auto prop = std::make_shared<UsdProxy::ProxyPropertyContext>();
            prop->setDefaultValue(PXR_NS::VtValue("TestToken"_tftoken))
                .setName(PXR_NS::TfToken("TestProp"_tftoken))
                .setType(PXR_NS::SdfSpecType::SdfSpecTypeAttribute);
            context->tryInsertProperty(prop);
        }
    };

    TEST(TestProxy, Decorator)
    {
        using namespace PXR_NS;
        using namespace UsdProxy;
        REGISTRY_PROXY_DECORATOR(TestDecorator);

        auto stage = UsdStage::CreateInMemory("test");

        UsdGeomXform::Define(stage, SdfPath("/root/Xform1"));
        UsdGeomXform::Define(stage, SdfPath("/root/Xform2"));
        UsdGeomXform::Define(stage, SdfPath("/root/Xform3"));

        for (auto prim : stage->TraverseAll())
        {
            auto proxy = UsdProxyPrim(prim);
            auto proxyProp = proxy.getProperty("TestProp"_tftoken);
            EXPECT_TRUE(proxyProp != nullptr);
            VtValue val;
            EXPECT_TRUE(proxyProp->getDefault(&val));
            EXPECT_TRUE(val.IsHolding<TfToken>());
            EXPECT_TRUE(val.Get<TfToken>() == "TestToken"_tftoken);
            proxyProp->setValue(VtValue("SecondTest"_tftoken));
        }

        for (UsdPrim prim : stage->TraverseAll())
        {
            auto attr = prim.GetAttribute("TestProp"_tftoken);
            EXPECT_TRUE(attr.IsValid());
            VtValue val;
            EXPECT_TRUE(attr.Get(&val));
            EXPECT_TRUE(val.IsHolding<TfToken>());
            EXPECT_TRUE(val.Get<TfToken>() == "SecondTest"_tftoken);
        }
    }


    TEST(TestProxy, Watcher)
    {
        using namespace PXR_NS;
        using namespace UsdProxy;

        auto stage = UsdStage::CreateInMemory("test");
        std::unordered_set<std::string> resyncPaths;

        StageObjectChangedWatcher watcher(stage, [&](UsdNotice::ObjectsChanged const& notice)
        {
            for (auto item : notice.GetChangedInfoOnlyPaths())
            {
                resyncPaths.emplace(item.GetPrimPath().GetString());
            }
            for (auto item : notice.GetResyncedPaths())
            {
                resyncPaths.emplace(item.GetPrimPath().GetString());
            }
        });

        UsdGeomXform::Define(stage, SdfPath("/root/Xform1"));
        UsdGeomXform::Define(stage, SdfPath("/root/Xform2"));
        UsdGeomXform::Define(stage, SdfPath("/root/Xform3"));

        std::unordered_set<std::string> expected =
        {
            "/root",
            "/root/Xform1",
            "/root/Xform2",
            "/root/Xform3"
        };

        EXPECT_TRUE(expected == resyncPaths);
    }

    TEST(TestProxy, CustomSchema)
    {
        using namespace PXR_NS;
        using namespace UsdProxy;

        auto plugins = PlugRegistry::GetInstance().RegisterPlugins("C:/dev/Projects/NauPrototype/build/win_vs2022_x64_dll/bin/Debug/plugins");
        for (auto& it : plugins)
            if (!it->IsLoaded())
            {
                it->Load();

            }
        //UsdSchemaRegistry::GetInstance().FindAndBuildAllSchemaDefinitions();
        //auto def = UsdSchemaRegistry::GetInstance().FindConcretePrimDefinition("NauAssetMesh"_tftoken);
        //auto propsNames = def->GetPropertyNames();

        auto stage = UsdStage::CreateInMemory("test");
        auto prim = stage->DefinePrim(SdfPath("/TestAsset"), "NauAssetMesh"_tftoken);
        auto isAct = prim.IsActive();
        auto isDef = prim.IsDefined();
        auto isValid = prim.IsValid();
        auto typeName = prim.GetTypeName().GetString();
        auto attrs = prim.GetAttributes();
        UsdProxyPrim proxy(prim);
        auto props = proxy.getProperties();

        auto is_empty = props.empty();

    }

}

int main(int argc, char** argv)
{
    //putenv("PXR_PLUGINPATH_NAME=C:/dev/Projects/NauPrototype/build/win_vs2022_x64_dll/exdeps/usd/plugin/usd;C:/dev/Projects/NauPrototype/build/win_vs2022_x64_dll/bin/Debug/plugins;");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}