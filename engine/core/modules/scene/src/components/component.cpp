// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/scene/components/component.h"

#include "nau/scene/components/component_life_cycle.h"
#include "nau/scene/scene_object.h"
#include "scene_management/scene_manager_impl.h"

namespace nau::scene
{
    Component::~Component()
    {
        NAU_ASSERT(m_asyncTasks.isEmpty());
        NAU_ASSERT(!m_parentObject);
    }

    Component::Component()
    {
        setUid(Uid::generate());
    }

    void Component::onBeforeDeleteObject()
    {
        if (m_parentObject)
        {
            m_parentObject->removeComponentFromList(*this);
        }
    }

    void Component::destroy()
    {
        getParentObject().removeComponent(*this);
    }

    SceneObject& Component::getParentObject() const
    {
        // In general m_parentObject can not be null.
        // But when component's deletion is requested and component deactivated (detached from scene hierarchy) it can be still alive
        // and accessible for user's logic (within not finished async operations).
        // Is such situations component can not interact with the scene and isOperable state must be checked.
        NAU_FATAL(m_parentObject, "Component object is not operable, please check component's state (isOperable)");
        return *m_parentObject;
    }

    bool Component::isOperable() const
    {
        return m_parentObject != nullptr;
    }

    ActivationState Component::getActivationState() const
    {
        return m_activationState;
    }

    void Component::changeActivationState(ActivationState newState)
    {
        [[maybe_unused]] const auto oldState = std::exchange(m_activationState, newState);

        if (m_activationState == ActivationState::Activating)
        {
            NAU_ASSERT(oldState == ActivationState::Inactive);
        }
        else if (m_activationState == ActivationState::Active)
        {
            NAU_ASSERT(oldState == ActivationState::Activating);

            m_sceneManager = &getServiceProvider().get<SceneManagerImpl>();

            if (auto* const componentEvents = as<IComponentEvents*>())
            {
                componentEvents->onComponentActivated();
            }
        }
        else if (m_activationState == ActivationState::Deactivating)
        {
            NAU_ASSERT(oldState == ActivationState::Active);
        }
        else if (m_activationState == ActivationState::Inactive)
        {
            NAU_ASSERT(oldState == ActivationState::Deactivating);
            if (auto* const componentEvents = as<IComponentEvents*>())
            {
                componentEvents->onComponentDeactivated();
            }
        }
    }

    async::Task<> Component::finalizeAsyncOperations()
    {
        co_await m_asyncTasks.awaitCompletion();
    }

    void Component::onThisValueChanged(std::string_view key)
    {
        if (m_activationState == ActivationState::Active)
        {
            NAU_FATAL(m_sceneManager);
            m_sceneManager->notifyListenerComponentWasChanged(*this);
        }
    }

    void Component::addRef()
    {
    }

    void Component::releaseRef()
    {
    }

    nau::IWeakRef* Component::getWeakRef()
    {
        NAU_FATAL("Weak referencing through IRefCounted api is not supported for this kind of object");
        return nullptr;
    }

    uint32_t Component::getRefsCount() const
    {
        return 1;
    }

}  // namespace nau::scene
