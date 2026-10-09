// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./asset_db_impl.h"
#include "./asset_file_content_provider.h"
#include "asset_manager_impl.h"
#include "nau/module/module.h"

namespace nau
{

    struct AssetsModule : nau::IModule
    {
        // Inherited via IModule
        nau::string getModuleName() override
        {
            return nau::string("AssetsModule");
        }
        void initialize() override
        {
            using namespace nau;

            NAU_MODULE_EXPORT_SERVICE(AssetManagerImpl);
            NAU_MODULE_EXPORT_SERVICE(AssetFileContentProvider);
            NAU_MODULE_EXPORT_SERVICE(AssetDBImpl);
        }
        void deinitialize() override
        {
        }
        void postInit() override
        {
        }
    };

}  // namespace nau

IMPLEMENT_MODULE(nau::AssetsModule);
