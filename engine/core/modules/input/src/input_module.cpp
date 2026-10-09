// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "input_manager.h"
#include "input_system_impl.h"
#include "nau/core_defines.h"
#include "nau/module/module.h"
#include "nau/service/service.h"
#include "nau/service/service_provider.h"

#ifdef NAU_PLATFORM_WIN32
    #include "platform/windows/input_msg_handler.h"
    #include "platform/windows/input_system_impl_win.h"
#endif

namespace nau::input
{
    /**
        @brief GlobalInputAutoUpdate to update global input manager
    */
    class GlobalInputAutoUpdate final : public IGamePreUpdate,
                                        public IServiceInitialization
    {
        NAU_RTTI_CLASS(nau::input::GlobalInputAutoUpdate, IGamePreUpdate, IServiceInitialization)

    private:
        void gamePreUpdate([[maybe_unused]] std::chrono::milliseconds dt) override
        {
            NAU_FATAL(m_inputManager);
            const float secondsDt = static_cast<float>(dt.count()) / 1000.f;

            // same as nau::input::update(dt), but a bit more efficient
            m_inputManager->update(secondsDt);
        }

        async::Task<> initService() override
        {
            m_inputManager = &getServiceProvider().get<input::InputManagerImpl>();
            return async::makeResolvedTask();
        }

        InputManagerImpl* m_inputManager = nullptr;
    };
}  // namespace nau::input

struct CoreInputModule : public nau::IModule
{
    // Inherited via IModule
    nau::string getModuleName() override
    {
        return nau::string(u8"CoreInput");
    }

    void initialize() override
    {
        NAU_MODULE_EXPORT_CLASS(nau::input::GlobalInputAutoUpdate);
        NAU_MODULE_EXPORT_SERVICE(nau::input::InputManagerImpl);
        NAU_MODULE_EXPORT_SERVICE(nau::InputSystemImpl);

#ifdef NAU_PLATFORM_WIN32
        NAU_MODULE_EXPORT_SERVICE(nau::input::WindowsInputMsgHandler);
        NAU_MODULE_EXPORT_SERVICE(nau::input::InputSystemImplWin);
#endif
    }

    void deinitialize() override
    {
    }

    void postInit() override
    {
    }
};

IMPLEMENT_MODULE(CoreInputModule);
