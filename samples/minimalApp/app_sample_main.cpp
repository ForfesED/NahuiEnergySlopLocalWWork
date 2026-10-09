// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/module/module_manager.h"

int main(int argc, char** argv)
{
    using namespace nau;
    using namespace std::chrono_literals;

    auto app = nau::createApplication(
        []
        {
            nau::loadModulesList(NAU_MODULES_LIST).ignore();

            getServiceProvider().addService(createPlatformWindowService());
            return nau::ResultSuccess;
        });

    app->startupOnCurrentThread();

    {
        auto& window = getServiceProvider().get<IWindowManager>().getActiveWindow();
        window.setVisible(true);
    }

    while(app->step())
    {
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
