// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/module/module.h"
#include "nau/rtti/rtti_impl.h"
#include "script_manager_impl.h"

namespace nau::scripts
{
    struct LuaScriptsModule : public nau::IModule
    {
        nau::string getModuleName() override
        {
            return "LuaModule";
        }

        void initialize() override
        {
            NAU_MODULE_EXPORT_SERVICE(nau::scripts::ScriptManagerImpl);
        }

        void deinitialize() override
        {
        }

        void postInit() override
        {
        }
    };

}  // namespace nau::scripts

IMPLEMENT_MODULE(nau::scripts::LuaScriptsModule)
