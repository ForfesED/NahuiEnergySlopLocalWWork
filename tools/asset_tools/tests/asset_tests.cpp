// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <gmock/gmock.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "nau/shared/args.h"
#include "nau/shared/file_system.h"
#include "nau/shared/logger.h"
#include "nau/shared/util.h"
#include "nau/usd_meta_tools/usd_meta_manager.h"
#include "nau/usd_meta_tools/usd_meta_generator.h"


#include "nlohmann/json.hpp"
#include "usd_proxy/usd_prim_proxy.h"
#include <pxr/base/plug/registry.h>
#include <pxr/base/plug/plugin.h>

namespace nau::test
{
    TEST(AssetTool, UsdMeta)
    {
        using namespace PXR_NS;
        using namespace UsdProxy;
        using namespace nau;

        nau::loadPlugins();
        auto stage = UsdStage::CreateInMemory("Test.usda");
        stage->DefinePrim(SdfPath("/TestAsset"), "NauAssetMesh"_tftoken);
        stage->DefinePrim(SdfPath("/TestAsset/SubAsset1"), "NauAssetMesh"_tftoken);
        stage->DefinePrim(SdfPath("/TestAsset/SubAsset2"), "NauAssetMesh"_tftoken);
        stage->DefinePrim(SdfPath("/TestAsset/SubAsset2/SubAsset3"), "NauAssetMesh"_tftoken);
        stage->DefinePrim(SdfPath("/TestTexture"), "NauAssetTexture"_tftoken);
        stage->DefinePrim(SdfPath("/TestShader"), "NauAssetShader"_tftoken);
        stage->DefinePrim(SdfPath("/TestMaterial"), "NauAssetMaterial"_tftoken);
        stage->DefinePrim(SdfPath("/TestSound"), "NauAssetSound"_tftoken);
        stage->DefinePrim(SdfPath("/TestVideo"), "NauAssetVideo"_tftoken);

        std::map<std::string, int> match
        {
            {"mesh", 4},
            {"texture", 1},
            {"material", 1},
            {"shader", 1},
            {"sound", 1},
            {"video", 1},
        };
        std::map<std::string, int> count;
        std::function<void(nau::UsdMetaInfo&)> traverse = [&](nau::UsdMetaInfo& info)
            {
                auto it = count.find(info.type);
                if (it == count.end())
                    count[info.type] = 0;
                count[info.type]++;

                for (auto it : info.children)
                    traverse(it);
            };
        auto info = UsdMetaManager::instance().getInfo(stage);
        for (auto it : info)
            traverse(it);

        EXPECT_EQ(match, count);
    }
    TEST(AssetTool, NauUsdFormat)
    {
        using namespace PXR_NS;
        using namespace UsdProxy;
        using namespace nau;

        nau::loadPlugins();

        std::filesystem::path p = std::filesystem::current_path() / "_temp";

        std::filesystem::create_directories(p);

        auto stage = UsdStage::CreateInMemory("Test.nausd");
        stage->DefinePrim(SdfPath("/TestTexture"), "NauAssetTexture"_tftoken);
        stage->GetRootLayer()->Export(p.string() + "/Test.nausd");

        auto loadedStage = UsdStage::Open(p.string() + "/Test.nausd");

        EXPECT_TRUE(loadedStage);

        std::filesystem::remove_all(p);
    }
}  // namespace nau::test