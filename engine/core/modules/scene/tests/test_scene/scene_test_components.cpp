// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "scene_test_components.h"

namespace nau::scene_test
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(MyDefaultSceneComponent)
    NAU_IMPLEMENT_DYNAMIC_OBJECT(MyDisposableComponent)
    NAU_IMPLEMENT_DYNAMIC_OBJECT(MyComponentWithAsyncUpdate)
    NAU_IMPLEMENT_DYNAMIC_OBJECT(MyCustomUpdateAction)

    WithDestructor::~WithDestructor()
    {
        if (m_onDestructorCallback)
        {
            m_onDestructorCallback();
        }
    }

    void WithDestructor::setOnDestructor(Functor<void()>&& callback)
    {
        m_onDestructorCallback = std::move(callback);
    }

    void WithDestructor::setOnDisposed(Functor<void()> callback)
    {
        m_onDisposedCallback = std::move(callback);
    }

    void WithDestructor::dispose()
    {
        if (m_onDisposedCallback)
        {
            m_onDisposedCallback();
        }
    }

    bool MyDefaultSceneComponent::isActivated() const
    {
        return m_activateWasCalled;
    }

    bool MyDefaultSceneComponent::isActivatedAsync() const
    {
        return m_activateAsyncWasCalled;
    }

    bool MyDefaultSceneComponent::isDeactivated() const
    {
        return m_deactivateWasCalled;
    }

    void MyDefaultSceneComponent::setBlockActivation(bool block)
    {
        m_activationIsBlocked = block;
    }

    void MyDefaultSceneComponent::setBlockDeletion(bool block)
    {
        m_deletionIsBlocked = block;

        if (m_deletionIsBlocked)
        {
            this->runAsync([this]() -> async::Task<>
            {
                for (; m_deletionIsBlocked;)
                {
                    co_await std::chrono::milliseconds(1);
                }
            });
        }
    }

    size_t MyDefaultSceneComponent::getUpdateCounter() const
    {
        return m_updateCounter;
    }

    void MyDefaultSceneComponent::onComponentActivated()
    {
        m_activateWasCalled = true;
    }

    async::Task<> MyDefaultSceneComponent::activateComponentAsync()
    {
        while (m_activationIsBlocked)
        {
            co_await std::chrono::milliseconds(1);
        }

        m_activateAsyncWasCalled = true;
    }

    void MyDefaultSceneComponent::activateComponent()
    {
        m_activateWasCalled = true;
    }

    void MyDefaultSceneComponent::deactivateComponent()
    {
        m_deactivateWasCalled = true;
    }

    void MyDefaultSceneComponent::updateComponent([[maybe_unused]] float dt)
    {
        ++m_updateCounter;
    }

    void MyDisposableComponent::setOnDeactivated(Functor<void()> callback)
    {
        m_onDeactivatedCallback = std::move(callback);
    }

    void MyDisposableComponent::setOnDestroyed(Functor<void()> callback)
    {
        m_onDestroyedCallback = std::move(callback);
    }

    void MyDisposableComponent::updateComponent([[maybe_unused]] float dt)
    {
    }

    void MyDisposableComponent::onComponentDeactivated()
    {
        if (m_onDeactivatedCallback)
        {
            m_onDeactivatedCallback();
        }
    }

    void MyDisposableComponent::onComponentDestroyed()
    {
        if (m_onDestroyedCallback)
        {
            m_onDestroyedCallback();
        }
    }

    async::Task<> MyComponentWithAsyncUpdate::updateComponentAsync(float dt)
    {
        ++m_updateAsyncCounter;


        while (m_asyncUpdateIsBlocked)
        {
            co_await async::Executor::getCurrent();
        }

        if (m_awaitTime.count() > 0)
        {
            co_await m_awaitTime;
            m_isAwaitCompleted = true;
            m_awaitTime = std::chrono::milliseconds(0);
        }
    }

    unsigned MyComponentWithAsyncUpdate::getUpdateAsyncCounter() const
    {
        return m_updateAsyncCounter;
    }

    void MyComponentWithAsyncUpdate::setAwaitTime(std::chrono::milliseconds time)
    {
        m_awaitTime = time;
        m_isAwaitCompleted = false;
    }

    bool MyComponentWithAsyncUpdate::isAwaitCompleted() const
    {
        return m_isAwaitCompleted;
    }

    void MyComponentWithAsyncUpdate::setBlockAsyncUpdate(bool block)
    {
        m_asyncUpdateIsBlocked = block;
    }


    async::Task<> MyCustomUpdateAction::updateComponentAsync(float dt)
    {
        if (auto action = std::exchange(m_asyncAction, nullptr); action)
        {
            co_await action(getParentObject());
        }
    }

    void MyCustomUpdateAction::setUpdateAsyncCallback(AsyncAction action)
    {
        m_asyncAction = std::move(action);
    }

    void registerAllTestComponentClasses()
    {
        auto& provider = getServiceProvider();
        provider.addClass<MyDefaultSceneComponent>();
        provider.addClass<MyDisposableComponent>();
        provider.addClass<MyComponentWithAsyncUpdate>();
        provider.addClass<MyCustomUpdateAction>();
    }

}  // namespace nau::scene_test
