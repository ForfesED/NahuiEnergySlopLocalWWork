// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/module/module.h"
#include "nau/service/service_provider.h"
#include "ui_manager.h"
#include "assets/ui_asset_view_factory.h"
#include <nau/core_defines.h>
#include "nau_backend/device_nau.h"
#include "nau/ui/components/ui_component.h"


struct UiModule : public nau::IModule
{
    nau::string getModuleName() override
    {
        return nau::string(u8"Ui");
    }

    void initialize() override
    {
        NAU_MODULE_EXPORT_SERVICE(nau::ui::UiManagerImpl);
        NAU_MODULE_EXPORT_SERVICE(nau::ui::data::UiAssetViewFactory);

        NAU_MODULE_EXPORT_CLASS(nau::ui::UiComponent);
    }

    void deinitialize() override
    {
    }

    void postInit() override
    {
    }
};

IMPLEMENT_MODULE(UiModule);
