// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "platform/windows/windows_window_manager_impl.h"
#include "nau/module/module.h"
#include "nau/service/service_provider.h"


struct AppWinModule : public nau::IModule
{
// Inherited via IModule
    nau::string getModuleName() override
    {
        return nau::string("AppWinModule");
    }
    void initialize() override
    {
        NAU_MODULE_EXPORT_CLASS(nau::WindowsWindowManager);
    }
    void deinitialize() override
    {
    }
    void postInit() override
    {
    }
};

IMPLEMENT_MODULE(AppWinModule);
