// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "vfx_impl.h"
#include "nau/module/module.h"
#include "components/vfx_component.h"

namespace nau
{
    struct VFXModule : public IModule
    {
        nau::string getModuleName() override
        {
            return "VFXModule";
        }
        void initialize() override
        {
            NAU_MODULE_EXPORT_SERVICE(nau::vfx::VFXManagerImpl);
            NAU_MODULE_EXPORT_CLASS(nau::vfx::VFXComponent);
        }
        void deinitialize() override
        {
        }
        void postInit() override
        {
        }
    };
}  // namespace nau

IMPLEMENT_MODULE(nau::VFXModule);
