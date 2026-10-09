// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <gmock/gmock.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <usd_uid_lookup/usd_uid_lookup.h>

#include <filesystem>
#include <string>

#include "nau/shared/args.h"
#include "nau/shared/file_system.h"
#include "nau/shared/logger.h"
#include "nau/shared/util.h"

namespace nau::test
{
    constexpr const char* testDbString = "{\"content\":[{\"dbPath\":\"0\\\\c0c6082b-92dc-11ef-9105-502f9ba726f4.gltf\",\"dirty\":false,\"lastModified\":133743283022216385,\"sourcePath\":\"models/helmet+[mesh_helmet_LP_13930damagedHelmet]\",\"type\":\"usda\",\"uid\":\"c0c6082b-92dc-11ef-9105-502f9ba726f4\"}]}";
    TEST(UsdUidLookup, UidLookupTest)
    {
        FileSystem fs;

        std::filesystem::path p = std::filesystem::current_path() / "_temp";
        std::filesystem::create_directories(p);

        fs.writeFile(p / "database.db", testDbString);

        nau::uid_lookup::IUidLookup& lookup = nau::uid_lookup::IUidLookup::getInstance();
        nau::Uid helmetUid = *nau::Uid::parseString("c0c6082b-92dc-11ef-9105-502f9ba726f4");

        EXPECT_TRUE(lookup.init(p.string() + "/database.db"));

        auto lookupResult = lookup.lookup(helmetUid);

        EXPECT_TRUE(!lookupResult.isError());
        EXPECT_TRUE(*lookupResult == "0\\c0c6082b-92dc-11ef-9105-502f9ba726f4.gltf");

        std::filesystem::remove_all(p);
    }
}  // namespace nau::test