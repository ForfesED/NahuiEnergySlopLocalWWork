// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/assets/asset_path.h"
#include "nau/test/helpers/assert_catcher_guard.h"

namespace nau::test
{
    class TestAssetPath : public testing::Test
    {
    };

    /**
     */
    TEST_F(TestAssetPath, ValidPath)
    {
        // Full asset path: scheme + container path + inner path
        ASSERT_TRUE(AssetPath::isValid("scheme:/any_asset-path+[inner/asset.path]"));

        // Asset path with no inner path
        ASSERT_TRUE(AssetPath::isValid("scheme:/any_asset-path"));

        // Asset path with empty inner path
        ASSERT_TRUE(AssetPath::isValid("scheme:/any_asset-path+[]"));
    }

    /**
     */
    TEST_F(TestAssetPath, InvalidPath)
    {
        // Empty string:
        ASSERT_FALSE(AssetPath::isValid(""));

        // No scheme
        ASSERT_FALSE(AssetPath::isValid("/any_asset-path+[inner_path]"));

        // Broken inner path
        ASSERT_FALSE(AssetPath::isValid("scheme:/any_asset-path+"));
        ASSERT_FALSE(AssetPath::isValid("scheme:/any_asset-path+["));
        ASSERT_FALSE(AssetPath::isValid("scheme:/any_asset-path+]"));
    }

    /**
     */
    TEST_F(TestAssetPath, ConstructFromValidString)
    {
        {
            AssetPath path{"scheme:/container_path/1+[]"};
            ASSERT_TRUE(path);

            // empty inner path expected to be eliminated
            ASSERT_EQ(path, AssetPath{"scheme:/container_path/1"});

            ASSERT_EQ(path.toString(), "scheme:/container_path/1");
            ASSERT_EQ(path.getScheme(), "scheme");
            ASSERT_EQ(path.getContainerPath(), "/container_path/1");
            ASSERT_EQ(path.getSchemeAndContainerPath(), "scheme:/container_path/1");
            ASSERT_TRUE(path.getAssetInnerPath().empty());
        }

        {
            AssetPath path{"scheme:/container_path/1+[inner.asset/path]"};
            ASSERT_EQ(path.toString(), "scheme:/container_path/1+[inner.asset/path]");
            ASSERT_TRUE(path);
            ASSERT_EQ(path.getScheme(), "scheme");
            ASSERT_EQ(path.getContainerPath(), "/container_path/1");
            ASSERT_EQ(path.getSchemeAndContainerPath(), "scheme:/container_path/1");
            ASSERT_EQ(path.getAssetInnerPath(), "inner.asset/path");
        }
    }

    /**
     */
    TEST_F(TestAssetPath, ConstructFromInvalidString)
    {
        const auto checkPathIsEmpty = [](const AssetPath& path)
        {
            ASSERT_FALSE(path);

            ASSERT_TRUE(path.toString().empty());
            ASSERT_TRUE(path.getScheme().empty());
            ASSERT_TRUE(path.getContainerPath().empty());
            ASSERT_TRUE(path.getSchemeAndContainerPath().empty());
            ASSERT_TRUE(path.getAssetInnerPath().empty());
        };

        AssertCatcherGuard assertGuard;

        // Empty string:
        ASSERT_NO_FATAL_FAILURE(checkPathIsEmpty(AssetPath{""}));

        // No scheme:
        ASSERT_NO_FATAL_FAILURE(checkPathIsEmpty(AssetPath{"/any_asset-path+[]"}));

        // Broken inner path:
        ASSERT_NO_FATAL_FAILURE(checkPathIsEmpty(AssetPath{"scheme:/any_asset-path+"}));

        ASSERT_EQ(assertGuard.assertFailureCounter, 3);
    }

    /**
     **/
    TEST_F(TestAssetPath, ConstructFromStrings)
    {
        {
            const AssetPath path0{"test", "container/path_1", "inner-path"};
            ASSERT_EQ(path0, AssetPath{"test:container/path_1+[inner-path]"});
        }

        {
            const AssetPath path0{"test", "container/path_1"};
            ASSERT_EQ(path0, AssetPath{"test:container/path_1"});
        }
    }

    /**
    */
    TEST_F(TestAssetPath, SetScheme)
    {
        AssetPath path{"test", "container/path_1", "inner-path"};

        path.setScheme("test_test");
        ASSERT_TRUE(path.hasScheme("test_test"));
        ASSERT_EQ(path, AssetPath{"test_test:container/path_1+[inner-path]"});

        path.setScheme("mini");
        ASSERT_TRUE(path.hasScheme("mini"));
        ASSERT_EQ(path, AssetPath{"mini:container/path_1+[inner-path]"});

        // dont allow to set empty scheme (and makes asset path invalid)
        AssertCatcherGuard assertGuard;
        path.setScheme("");
        ASSERT_TRUE(path.hasScheme("mini"));
        ASSERT_EQ(path, AssetPath{"mini:container/path_1+[inner-path]"});
        ASSERT_EQ(assertGuard.assertFailureCounter, 1);
    }

    /**
    */
    TEST_F(TestAssetPath, SetContainerPath)
    {
        AssetPath path{"test", "container/path_1", "inner-path"};
        path.setContainerPath("container/new/path_2");
        ASSERT_EQ(path, AssetPath{"test:container/new/path_2+[inner-path]"});

        path.setContainerPath("mini_path");
        ASSERT_EQ(path, AssetPath{"test:mini_path+[inner-path]"});

        // dont allow to set empty container path (and makes asset path invalid)
        AssertCatcherGuard assertGuard;
        path.setContainerPath("");
        ASSERT_EQ(path, AssetPath{"test:mini_path+[inner-path]"});
        ASSERT_EQ(assertGuard.assertFailureCounter, 1);
    }

    /**
    */
    TEST_F(TestAssetPath, SetAssetInnerPath)
    {
        AssetPath path{"test", "container/path_1", "asset_1"};
        ASSERT_EQ(path, AssetPath{"test:container/path_1+[asset_1]"});

        // set new inner path (with greater size)  (when current inner path exists)
        path.setAssetInnerPath("asset_asset_2");
        ASSERT_EQ(path, AssetPath{"test:container/path_1+[asset_asset_2]"});

        // set new inner path (with lesser size)
        path.setAssetInnerPath("mini");
        ASSERT_EQ(path, AssetPath{"test:container/path_1+[mini]"});

        // clear inner path
        path.setAssetInnerPath("");
        ASSERT_EQ(path, AssetPath{"test:container/path_1"});

        // set new inner path (with greater size)  (when current inner path not exists)
        path.setAssetInnerPath("asset_1");
        ASSERT_EQ(path, AssetPath{"test:container/path_1+[asset_1]"});
    }

    TEST_F(TestAssetPath, CopyConstruct)
    {
        AssetPath path1{"test", "container/path_1", "asset_1"};
        AssetPath path2 = path1;

        ASSERT_TRUE(path1);
        ASSERT_EQ(path2, path1);
    }

    TEST_F(TestAssetPath, MoveConstruct)
    {
        AssetPath path0{"test", "container/path_1", "asset_1"};
        AssetPath path1 = path0;
        AssetPath path2 = std::move(path1);

        ASSERT_FALSE(path1);
        ASSERT_EQ(path2, path0);
    }

    TEST_F(TestAssetPath, CopyAssign)
    {
        AssetPath path1{"test", "container/path_1", "asset_1"};
        AssetPath path2;
            
        path2 = path1;

        ASSERT_TRUE(path1);
        ASSERT_EQ(path2, path1);
    }

    TEST_F(TestAssetPath, MoveAssign)
    {
        AssetPath path0{"test", "container/path_1", "asset_1"};
        AssetPath path1 = path0;
        AssetPath path2;

        path2 = std::move(path1);

        ASSERT_FALSE(path1);
        ASSERT_EQ(path2, path0);
    }

}  // namespace nau::test
