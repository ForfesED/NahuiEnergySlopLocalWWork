// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./logging_service.h"
#include "./main_loop/main_loop_service.h"
#include "nau/app/application.h"
#include "nau/app/main_loop/game_system.h"
#include "nau/async/work_queue.h"
#include "nau/memory/singleton_memop.h"
#include "nau/messaging/messaging.h"
#include "nau/module/module_manager.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/runtime/internal/runtime_state.h"
#include "nau/scene/internal/scene_manager_internal.h"
#include "nau/utils/stopwatch.h"
#include "nau/vfx_manager.h"


namespace nau
{

    namespace ui
    {
        struct UiManager;
    }

    /**
     */
    class ApplicationImpl final : public Application
    {
        NAU_RTTI_CLASS(nau::Application, Application)
        NAU_DECLARE_SINGLETON_MEMOP(ApplicationImpl)

    public:
        ApplicationImpl();

        ~ApplicationImpl();

        void startupOnCurrentThread() override;
        bool isMainThread() override;

        bool step() override;

        void stop() override;

        bool isClosing() const override;
        async::Executor::Ptr getExecutor() override;
        bool hasExecutor() override;

    private:
        enum class AppState
        {
            Active,
            ShutdownRequested,
            GameShutdownProcessed,
            RuntimeShutdownProcessed,
            ShutdownCompleted
        };

        void shutdownCoreServices();

        Result<> startupServices();

        async::Task<> shutdownRuntime();

        void completeShutdown();

        void mainGameStep(float dt);

        RuntimeState::Ptr m_runtime = RuntimeState::create();
        IModuleManager::Ptr m_moduleManager = createModuleManager();
        WorkQueue::Ptr m_appWorkQueue;

        std::thread::id m_hostThreadId;
        std::atomic<AppState> m_appState = AppState::Active;

        MainLoopService* m_mainLoop = nullptr;

        ui::UiManager* m_uiManager = nullptr;
        vfx::VFXManager* m_vfxManager = nullptr;

        async::Task<> m_shutdownTask;
        Functor<bool()> m_runtimeShutdown;
        nau::TickStopwatch m_tickStopwatch;
    };
}  // namespace nau
