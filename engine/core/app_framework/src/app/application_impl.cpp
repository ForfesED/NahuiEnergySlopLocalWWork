// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./application_impl.h"

#include "nau/diag/device_error.h"
#include "nau/service/internal/service_provider_initialization.h"
#include "nau/service/service_provider.h"
#include "nau/ui.h"


namespace nau
{
    ApplicationImpl::ApplicationImpl()
    {
        NAU_ASSERT(!applicationExists());
        setApplication(this);

        m_moduleManager->doModulesPhase(IModuleManager::ModulesPhase::Init);
        getServiceProvider().addService<MainLoopService>();
    }

    ApplicationImpl::~ApplicationImpl()
    {
        NAU_ASSERT(applicationExists());
        setApplication(nullptr);
    }

    bool ApplicationImpl::isClosing() const
    {
        return m_appState != AppState::Active;
    }

    bool ApplicationImpl::hasExecutor()
    {
        return m_appWorkQueue != nullptr;
    }

    async::Executor::Ptr ApplicationImpl::getExecutor()
    {
        NAU_ASSERT(hasExecutor());
        return m_appWorkQueue;
    }

    void ApplicationImpl::shutdownCoreServices()
    {
        // 1. destroying services prior modules (because services are belongs to modules).
        setDefaultServiceProvider(nullptr);

        // 2. unload modules
        m_moduleManager->doModulesPhase(IModuleManager::ModulesPhase::Cleanup);
        m_moduleManager.reset();

        // 3. de-initialize diagnostics
        diag::setDeviceError(nullptr);
    }

    Result<> ApplicationImpl::startupServices()
    {
        const auto waitTaskAndPoll = [&](async::Task<> task) -> Result<>
        {
            while (!task.isReady())
            {
                m_appWorkQueue->poll();
            }

            return !task.isRejected() ? Result<>{} : task.getError();
        };

        ServiceProvider& serviceProvider = getServiceProvider();

        auto& serviceProviderInit = serviceProvider.as<core_detail::IServiceProviderInitialization&>();

        // TODO: check preInit result
        NauCheckResult(waitTaskAndPoll(serviceProviderInit.preInitServices()))

        // TODO: check init result
        NauCheckResult(waitTaskAndPoll(serviceProviderInit.initServices()))

        m_mainLoop = &serviceProvider.get<MainLoopService>();

        if (getServiceProvider().has<ui::UiManager>())
        {
            m_uiManager = &getServiceProvider().get<ui::UiManager>();
        }

        if (getServiceProvider().has<vfx::VFXManager>())
        {
            m_vfxManager = &getServiceProvider().get<vfx::VFXManager>();
        }

        return {};
    }

    void ApplicationImpl::startupOnCurrentThread()
    {
        NAU_ASSERT(m_hostThreadId == std::thread::id{});

        m_hostThreadId = std::this_thread::get_id();
        m_appWorkQueue = WorkQueue::create();
        m_appWorkQueue->setName("App Work Queue");

        async::Executor::setThisThreadExecutor(m_appWorkQueue);

        startupServices().ignore();
    }

    bool ApplicationImpl::isMainThread()
    {
        NAU_ASSERT(m_hostThreadId != std::thread::id{});

        return m_hostThreadId == std::this_thread::get_id();
    }

    bool ApplicationImpl::step()
    {
        NAU_ASSERT(m_hostThreadId == std::this_thread::get_id(), "Invalid thread");
        if (m_appState == AppState::ShutdownCompleted)
        {
            return false;
        }

        const float dt = m_tickStopwatch.tick();
        m_appWorkQueue->poll();

        if (m_appState == AppState::Active)
        {
            mainGameStep(dt);
        }
        else if (m_appState == AppState::ShutdownRequested)
        {
            NAU_ASSERT(!m_shutdownTask);
            NAU_FATAL(m_mainLoop);

            m_appState = AppState::GameShutdownProcessed;
            m_shutdownTask = m_mainLoop->shutdownMainLoop();
        }
        else if (m_appState == AppState::GameShutdownProcessed)
        {
            NAU_FATAL(m_shutdownTask);
            if (!m_shutdownTask.isReady())
            {
                mainGameStep(dt);
            }
            else
            {
                m_shutdownTask = nullptr;
                m_shutdownTask = shutdownRuntime();
            }
        }
        else if (m_appState == AppState::RuntimeShutdownProcessed)
        {
            NAU_FATAL(m_runtimeShutdown);
            NAU_FATAL(m_shutdownTask);

            if (!m_runtimeShutdown())
            {
                NAU_ASSERT(m_shutdownTask.isReady());
                completeShutdown();
            }
        }

        return m_appState != AppState::ShutdownCompleted;
    }

    void ApplicationImpl::stop()
    {
        [[maybe_unused]] AppState expectedState = AppState::Active;
        m_appState.compare_exchange_strong(expectedState, AppState::ShutdownRequested);
    }

    async::Task<> ApplicationImpl::shutdownRuntime()
    {
        [[maybe_unused]] const auto oldAppState = m_appState.exchange(AppState::RuntimeShutdownProcessed);
        NAU_ASSERT(oldAppState == AppState::GameShutdownProcessed);

        async::Task<> shutdownServicesTask = getServiceProvider().as<core_detail::IServiceProviderInitialization&>().shutdownServices();
        m_runtimeShutdown = m_runtime->shutdown(false);
        co_await shutdownServicesTask;
    }

    void ApplicationImpl::completeShutdown()
    {
        [[maybe_unused]] const auto oldAppState = m_appState.exchange(AppState::ShutdownCompleted);
        NAU_ASSERT(oldAppState == AppState::RuntimeShutdownProcessed);

        shutdownCoreServices();
    }

    void ApplicationImpl::mainGameStep(float dt)
    {
        NAU_FATAL(m_mainLoop);

        if (m_uiManager)
        {
            m_uiManager->update(dt);
        }

        if (m_vfxManager)
        {
            m_vfxManager->update(dt);
        }

        m_mainLoop->doGameStep(dt);
#if 0
        if (m_sceneManager != nullptr)
        {
            m_sceneManager->update(dt);

            if (imgui_get_state() != ImGuiState::OFF)
            {
                imgui_cache_render_data();
                imgui_update();
            }
        }
#endif
    }

}  // namespace nau
