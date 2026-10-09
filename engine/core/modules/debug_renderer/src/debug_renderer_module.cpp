// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// assets_module.cpp


#include "debug_renderer_sys_impl.h"
#include "nau/module/module.h"

namespace nau
{

    struct DebugRendererModule : nau::IModule
    {
        // Inherited via IModule
        nau::string getModuleName() override
        {
            return nau::string("DebugRenderer");
        }
        void initialize() override
        {
            using namespace nau;

            NAU_MODULE_EXPORT_SERVICE(DebugRenderSysImpl);
        }
        void deinitialize() override
        {
        }
        void postInit() override
        {
        }
    };

}  // namespace nau

IMPLEMENT_MODULE(nau::DebugRendererModule);
