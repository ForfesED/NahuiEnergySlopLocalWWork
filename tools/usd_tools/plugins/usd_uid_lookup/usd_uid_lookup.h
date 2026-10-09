// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <filesystem>

#include "nau/utils/uid.h"
#include "usd_uid_lookup_api.h"

namespace nau
{
    namespace uid_lookup
    {
        class USD_UID_LOOKUP_API IUidLookup
        {
        public:
            static IUidLookup& getInstance();

            virtual bool init(std::filesystem::path assetDbPath) = 0;
            virtual bool unload(std::filesystem::path assetDbPath) = 0;
            virtual nau::Result<std::string> lookup(const nau::Uid& uid) = 0;
        };
    }  // namespace uid_lookup
}  // namespace nau

extern "C"
{
    USD_UID_LOOKUP_API nau::uid_lookup::IUidLookup& getUsdUidLookup();
}