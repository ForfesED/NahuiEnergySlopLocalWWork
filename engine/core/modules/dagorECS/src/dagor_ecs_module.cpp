// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/module/module.h"
#include "nau/service/service_provider.h"


struct CoreDarorECSModule : public nau::IModule
{
// Inherited via IModule
    nau::string getModuleName() override
    {
        return nau::string(u8"DagorECS");
    }

    void initialize() override
    {
    }

    void deinitialize() override
    {
    }

    void postInit() override
    {
    }
};

IMPLEMENT_MODULE(CoreDarorECSModule);

