// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <EASTL/map.h>
#include <EASTL/unordered_map.h>
#include <EASTL/set.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/vector.h>
#include <gainput/gainput.h>

#include "nau/app/main_loop/game_system.h"
#include "nau/input_system.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/service/service.h"
#include "nau/utils/functor.h"
#include "signals/input_signals_impl.h"

namespace nau
{
    class InputSystemImpl final : public IServiceInitialization,
                                  public IInputSystem,
                                  public IGamePreUpdate
    {
        NAU_RTTI_CLASS(InputSystemImpl, IServiceInitialization, IInputSystem, IGamePreUpdate)
    public:
        InputSystemImpl();

        virtual eastl::shared_ptr<IInputAction> addAction(const eastl::string& serialized, nau::Functor<void(IInputSignal*)> actionCallback) override;
        virtual eastl::shared_ptr<IInputAction> addAction(const eastl::string& name, IInputAction::Type type, IInputSignal* signal, nau::Functor<void(IInputSignal*)> actionCallback) override;
        virtual bool removeAction(eastl::shared_ptr<IInputAction>&& action) override;

        virtual eastl::shared_ptr<IInputAction> loadAction(const io::FsPath& filePath, nau::Functor<void(IInputSignal*)> actionCallback) override;
        virtual bool saveAction(const eastl::shared_ptr<IInputAction> action, const eastl::string& filePath) override;
        virtual void getActions(eastl::vector<eastl::shared_ptr<IInputAction>>& actions) override;

        virtual IInputSignal* createSignal(const eastl::string& signalType) override;
        virtual IInputSignal* createSignal(const eastl::string& signalType, const eastl::string& controllerName, nau::Functor<void(IInputSignal*)> signal) override;

        virtual void setContext(const eastl::string& context) override;
        virtual void addContext(const eastl::string& context) override;
        virtual void removeContext(const eastl::string& context) override;

        IInputController* getController(const eastl::string& controllerDesc) override;
        eastl::vector<IInputDevice*> getDevices() override;

        void gamePreUpdate(std::chrono::milliseconds dt) override;

        gainput::InputManager& getGainput()
        {
            return m_inputManager;
        }
        void setInputSource(const eastl::string& source) override;

    private:
        class InputSignalFactory
        {
        public:
            static InputSignalImpl* create(const eastl::string& type);
        };
        gainput::InputManager m_inputManager;
        eastl::vector<eastl::shared_ptr<IInputDevice>> m_devices;
        eastl::unordered_map<eastl::string, eastl::shared_ptr<IInputController>> m_controllers;
        eastl::multimap<eastl::string, eastl::shared_ptr<IInputAction>> m_actions;
        eastl::set<eastl::string> m_contexts;
        
        eastl::string m_currentSource;
        eastl::set<eastl::string> m_sources;
    };
}  // namespace nau
