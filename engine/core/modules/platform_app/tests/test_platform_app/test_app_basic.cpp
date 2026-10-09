// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/app/core_window_manager.h"
#include "nau/app/platform_window.h"
#include "nau/module/module_manager.h"
#include "nau/runtime/disposable.h"
#include "nau/service/service_provider.h"
#include "nau/threading/event.h"
#include "nau/threading/set_thread_name.h"
#include <nau/io/file_system.h>

namespace nau::test
{
    // TODO NAU-2089
    /*
    TEST(TestPlatformApp, Test1)
    {
        GTEST_SKIP_("Test is broken. Considered to be fixed or removed");

        using namespace nau::async;

        setDefaultServiceProvider(createServiceProvider());

        getServiceProvider().addService(nau::io::createNativeFileSystem("./"));

        auto manager = createModuleManager();
        loadModulesList(NAU_MODULES_LIST).ignore();

        manager->doModulesPhase(IModuleManager::ModulesPhase::Init);

        threading::Event signal;

        std::thread thread([&signal]
                           {
                               threading::setThisThreadName("PlatformApp");

                               auto classes = getServiceProvider().findClasses<ICoreWindowManager>();
                               IClassDescriptor::Ptr classDesc = classes.front();

                               nau::Ptr<ICoreWindowManager> app = classDesc->getConstructor()->invokeToPtr(nullptr, {});
                               getServiceProvider().addService(app);

                               app->bindToCurrentThread();

                               signal.set();

                               app->getActiveWindow().setVisible(true);

                               while(app->pumpMessageQueue(true))
                               {
                               }
                           });

        signal.wait();
        //
        //[]() -> Task<>
        //{
        //    //ASYNC_SWITCH_EXECUTOR(getServiceProvider().get<ICire>().getExecutor());
        //    std::cout << "Do APP JOB\n";
        //}().detach();

        getServiceProvider().get<ICoreWindowManager>().as<IDisposable&>().dispose();

        thread.join();
    }
    */

}  // namespace nau::test
