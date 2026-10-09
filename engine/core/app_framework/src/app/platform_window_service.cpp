// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./platform_window_service.h"

#include "nau/app/app_messages.h"
#include "nau/app/application.h"
#include "nau/app/application_services.h"
#include "nau/app/core_window_manager.h"
#include "nau/service/service_provider.h"
#include "nau/threading/event.h"
#include "nau/threading/set_thread_name.h"

namespace nau
{
    PlatformWindowService::~PlatformWindowService()
    {
        NAU_ASSERT(!m_platformAppCompletedTask || m_platformAppCompletedTask.isReady());
    }

    async::Task<> PlatformWindowService::preInitService()
    {
        using namespace nau::async;

        auto platformAppClasses = getServiceProvider().findClasses<ICoreWindowManager>();
        if (platformAppClasses.empty())
        {
            // LOG: NO Platform App module found
            co_return;  // return Error ?
        }

        auto& appClass = platformAppClasses.front();
        NAU_FATAL(appClass->getConstructor());

        nau::Ptr<> platformApp = rtti::TakeOwnership{(*appClass->getConstructor()->invoke(nullptr, {}))->as<IRefCounted*>()};
        NAU_FATAL(platformApp);

        TaskSource<> appReady;
        Task<> appReadyTask = appReady.getTask();

        m_platformAppThread = std::thread(
            [this](nau::Ptr<ICoreWindowManager> platformApp, async::TaskSource<> appReady)
        {
            threading::setThisThreadName("PlatformApp");

            TaskSource<> appCompleted;
            m_platformAppCompletedTask = appCompleted.getTask();
            scope_on_leave
            {
                appCompleted.resolve();
            };

            getServiceProvider().addService(platformApp);
            platformApp->bindToCurrentThread();
            appReady.resolve();

            Result<> result;
            do
            {
                result = platformApp->pumpMessageQueue(true);
            } while (result);
        },
            std::move(platformApp), std::move(appReady));

        co_await appReadyTask;

        m_messageSubscriptions.emplace_back(
            AppWindowClosed.subscribe(getBroadcaster(), []
        {
            getApplication().stop();
        }));
    }

    async::Task<> PlatformWindowService::initService()
    {
        return async::Task<>::makeResolved();
    }

    async::Task<> PlatformWindowService::disposeAsync()
    {
        if (auto* const disposable = getServiceProvider().get<ICoreWindowManager>().as<IDisposable*>())
        {
            disposable->dispose();
        }

        NAU_ASSERT(m_platformAppCompletedTask);
        co_await m_platformAppCompletedTask;

        if (m_platformAppThread.joinable())
        {
            m_platformAppThread.join();
        }
    }

    eastl::unique_ptr<IRttiObject> createPlatformWindowService()
    {
        return eastl::make_unique<PlatformWindowService>();
    }

}  // namespace nau
