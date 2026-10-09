// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_uid_lookup.h"

#include <json/json.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

#include "nau/assets/asset_meta_info.h"
#include "nau/serialization/json.h"
#include "nau/serialization/json_utils.h"
#include "nau/serialization/runtime_value_builder.h"
#include "nau/serialization/serialization.h"

namespace nau
{
    namespace uid_lookup
    {
        namespace fs = std::filesystem;

        void readFile(fs::path path, std::stringstream& ss)
        {
            std::ifstream t(path);
            ss << t.rdbuf();
        }

        // For internal usage only
        struct AssetDbInternal
        {
            std::vector<AssetMetaInfoBase> content;
            fs::path path;
            size_t size;
            unsigned long long lastWriteTime;
            NAU_CLASS_FIELDS(
                CLASS_FIELD(content))
        };

        class USD_UID_LOOKUP_API UsdUidLookupImpl final : public IUidLookup
        {
        public:
            virtual bool init(std::filesystem::path assetDbPath) override;
            virtual bool unload(std::filesystem::path assetDbPath) override;
            virtual nau::Result<std::string> lookup(const nau::Uid& uid) override;

        private:
            void checkIsDirty();

            std::unordered_map<fs::path, AssetDbInternal> m_lookupTable;
        };

        bool UsdUidLookupImpl::init(fs::path assetDbPath)
        {
            NAU_VERIFY(fs::exists(assetDbPath), "Asset database path does not exist!");
            NAU_ASSERT(m_lookupTable.find(assetDbPath) == m_lookupTable.end(), "Asset database path already exists!");

            std::stringstream ss;
            readFile(assetDbPath, ss);

            NAU_ASSERT(!ss.str().empty(), "Asset database is empty!");

            auto result = serialization::JsonUtils::parse<AssetDbInternal>(ss.str());

            if (result.isError())
            {
                return false;
            }

            m_lookupTable[assetDbPath] = *result;
            m_lookupTable[assetDbPath].lastWriteTime = fs::last_write_time(assetDbPath).time_since_epoch().count();
            m_lookupTable[assetDbPath].path = assetDbPath;
            m_lookupTable[assetDbPath].size = result->content.size();

            return true;
        }

        bool UsdUidLookupImpl::unload(std::filesystem::path assetDbPath)
        {
            NAU_ASSERT(m_lookupTable.find(assetDbPath) != m_lookupTable.end(), "Asset database path does not exist!");
            m_lookupTable.erase(assetDbPath);
            return true;
        }

        nau::Result<std::string> UsdUidLookupImpl::lookup(const nau::Uid& uid)
        {
            if (m_lookupTable.empty())
            {
                return NauMakeError("Asset database is empty!");
            }

            checkIsDirty();

            for (auto& [assetDbPath, assetDb] : m_lookupTable)
            {
                for (auto& metaInfo : assetDb.content)
                {
                    if (metaInfo.uid == uid)
                    {
                        return std::string(metaInfo.dbPath.begin(), metaInfo.dbPath.end());
                    }
                }
            }

            return NauMakeError("Asset with uid {} coult not be found!", toString(uid));
        }

        void UsdUidLookupImpl::checkIsDirty()
        {
            for (auto it = m_lookupTable.begin(); it != m_lookupTable.end();)
            {
                auto lastModifyTime = fs::last_write_time(it->first).time_since_epoch().count();
                auto size = it->second.size;

                if (lastModifyTime > it->second.lastWriteTime)
                {
                    std::stringstream ss;
                    readFile(it->first, ss);

                    if (ss.str().empty())
                    {
                        it = m_lookupTable.erase(it);
                        continue;
                    }

                    auto result = serialization::JsonUtils::parse<AssetDbInternal>(ss.str());

                    if (result.isError())
                    {
                        it = m_lookupTable.erase(it);
                        continue;
                    }

                    it->second = *result;
                    it->second.lastWriteTime = lastModifyTime;
                    it->second.path = it->first;
                    it->second.size = result->content.size();
                }
                else
                {
                    ++it;
                }
            }
        }

        IUidLookup& IUidLookup::getInstance()
        {
            static UsdUidLookupImpl instance;

            return instance;
        }
    }  // namespace uid_lookup
}  // namespace nau

USD_UID_LOOKUP_API nau::uid_lookup::IUidLookup& getUsdUidLookup()
{
    return nau::uid_lookup::IUidLookup::getInstance();
}
