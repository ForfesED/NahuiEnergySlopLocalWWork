// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/assets/asset_ref.h"
#include "nau/test/helpers/app_guard.h"

namespace nau::test
{
    TEST(TestAssetRef, ParseEmptyStringIsOk)
    {
        AssetRef<> assetRef;
        Result<> parseResult = parse("", assetRef);
        ASSERT_TRUE(parseResult);
        ASSERT_FALSE(assetRef);
    }

    TEST(TestAssetRef, ConstructFromCharArray)
    {
        AppGuard app;
        app.start();
        scope_on_leave
        {
            app.stop();
        };

        AssetRef<> assetRef("file:/content/white_8x8.png", true);
        ASSERT_TRUE(assetRef);
    }

}  // namespace nau::test
