// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/runtime/disposable.h"
#include "nau/scene/components/component_life_cycle.h"
#include "nau/scene/components/scene_component.h"

namespace nau::scene_test
{
    /**
     */
    class WithDestructor : public virtual IDisposable
    {
    public:
        NAU_TYPEID(WithDestructor)
        NAU_CLASS_BASE(IDisposable)

        ~WithDestructor();

        void setOnDestructor(Functor<void()>&& callback);
        void setOnDisposed(Functor<void()> callback);

    private:
        void dispose() override;

        Functor<void()> m_onDestructorCallback;
        Functor<void()> m_onDisposedCallback;
    };

    /**
     */
    class MyDefaultSceneComponent final : public scene::SceneComponent,
                                          public scene::IComponentEvents,
                                          public scene::IComponentActivation,
                                          public scene::IComponentUpdate,
                                          public WithDestructor
    {
        NAU_OBJECT(MyDefaultSceneComponent,
                   scene::SceneComponent,
                   scene::IComponentEvents,
                   scene::IComponentActivation,
                   scene::IComponentUpdate,
                   WithDestructor)

        NAU_DECLARE_DYNAMIC_OBJECT

    public:
        bool isActivated() const;

        bool isActivatedAsync() const;

        bool isDeactivated() const;

        size_t getDeactivatedCounter() const;

        void setBlockActivation(bool block);

        void setBlockDeletion(bool block);

        size_t getUpdateCounter() const;

    private:
        void onComponentActivated() override;

        async::Task<> activateComponentAsync() override;

        void activateComponent() override;

        void deactivateComponent() override;

        void updateComponent(float dt) override;

        bool m_activateWasCalled = false;
        bool m_activateAsyncWasCalled = false;
        bool m_deactivateWasCalled = false;

        bool m_activationIsBlocked = false;
        bool m_deletionIsBlocked = false;
        size_t m_updateCounter = 0;
    };

    /**
     */
    class MyDisposableComponent final : public scene::SceneComponent,
                                        public scene::IComponentEvents,
                                        public scene::IComponentUpdate,
                                        public WithDestructor
    {
        NAU_OBJECT(MyDisposableComponent,
                   scene::SceneComponent,
                   scene::IComponentEvents,
                   scene::IComponentUpdate,
                   WithDestructor)

        NAU_DECLARE_DYNAMIC_OBJECT

    public:
        void setOnDeactivated(Functor<void()> callback);
        void setOnDestroyed(Functor<void()> callback);

    private:
        void updateComponent(float dt) override;
        void onComponentDeactivated() override;
        void onComponentDestroyed() override;

        Functor<void()> m_onDeactivatedCallback;
        Functor<void()> m_onDestroyedCallback;
    };

    /**
     */
    class MyComponentWithAsyncUpdate final : public scene::SceneComponent,
                                             public scene::IComponentAsyncUpdate
    {
        NAU_OBJECT(MyComponentWithAsyncUpdate, scene::SceneComponent, scene::IComponentAsyncUpdate)
        NAU_DECLARE_DYNAMIC_OBJECT

    public:
        async::Task<> updateComponentAsync(float dt) override;

        unsigned getUpdateAsyncCounter() const;

        void setAwaitTime(std::chrono::milliseconds time);

        bool isAwaitCompleted() const;

        void setBlockAsyncUpdate(bool blocked);

    private:
        unsigned m_updateAsyncCounter = 0;
        std::chrono::milliseconds m_awaitTime{2};
        bool m_asyncUpdateIsBlocked = false;
        bool m_isAwaitCompleted = false;
    };

    /**
     */
    class MyCustomUpdateAction final : public scene::SceneComponent,
                                       public scene::IComponentAsyncUpdate
    {
        NAU_OBJECT(MyCustomUpdateAction, scene::SceneComponent, scene::IComponentAsyncUpdate)
        NAU_DECLARE_DYNAMIC_OBJECT

    public:
        using AsyncAction = Functor<async::Task<>(scene::SceneObject&)>;

        async::Task<> updateComponentAsync(float dt) override;

        void setUpdateAsyncCallback(AsyncAction action);

    private:
        AsyncAction m_asyncAction;
    };

    void registerAllTestComponentClasses();
}  // namespace nau::scene_test
