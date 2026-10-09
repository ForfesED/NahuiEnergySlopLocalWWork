// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/async/task.h"
#include "nau/rtti/type_info.h"

namespace nau::scene
{
    /**
     * @brief Provides an interface for component per-frame update.
     *
     * See also: IComponentAsyncUpdate.
     */
    struct NAU_ABSTRACT_TYPE IComponentUpdate
    {
        NAU_TYPEID(nau::scene::IComponentUpdate)

        /**
         * @brief Destructor.
         */
        virtual ~IComponentUpdate() = default;

        /**
         * @brief Updates the component.
         *
         * @param [int] dt  Elapsed time.
         *
         * @warning The update loop waits for all IComponentUpdate::updateComponent and
         *          IComponentAsyncUpdate::updateComponentAsync calls completion, until it can proceed to the next frame.
         *          Consequently, all lasting operations are to be avoided within these calls. Instead, address to
         *          IComponentAsyncListener::listenComponent.
         */
        virtual void updateComponent(float dt) = 0;
    };

    /**
     * @brief Provides an interface for component per-frame asynchronous update.
     *
     * See also: IComponentUpdate.
     */
    struct NAU_ABSTRACT_TYPE IComponentAsyncUpdate
    {
        NAU_TYPEID(nau::scene::IComponentAsyncUpdate)

        /**
         * @brief Destructor.
         */
        virtual ~IComponentAsyncUpdate() = default;

        /**
         * @brief Schedules the component update operation.
         *
         * @param [in] dt   Elapsed time.
         * @return          Task object that provides the operation status.
         *
         * @warning The update loop waits for all IComponentUpdate::updateComponent and
         *          IComponentAsyncUpdate::updateComponentAsync calls completion, until it can proceed to the next frame.
         *          Consequently, all lasting operations are to be avoided within these calls. Instead, address to
         *          IComponentAsyncListener::listenComponent.
         */
        virtual async::Task<> updateComponentAsync(float dt) = 0;
    };

    /**
     * @brief Provides an interface for component activation & deactivation.
     *
     * See also: IComponentAsyncActivation.
     */
    struct NAU_ABSTRACT_TYPE IComponentActivation
    {
        NAU_TYPEID(nau::scene::IComponentActivation)

        /**
         * @brief Destructor.
         */
        virtual ~IComponentActivation() = default;

        /**
         * @brief Activates the component.
         */
        virtual void activateComponent()
        {
        }

        virtual async::Task<> activateComponentAsync()
        {
            return async::Task<>::makeUninitialized();
        }

        /**
         * @brief Deactivates the component.
         */
        virtual void deactivateComponent()
        {
        }
    };

    /**
     * @brief Provides an interface for events that are triggered during the component lifecycle.
     */
    struct NAU_ABSTRACT_TYPE IComponentEvents
    {
        NAU_TYPEID(nau::scene::IComponentEvents)

        /**
         * @brief Destructor.
         */
        virtual ~IComponentEvents() = default;

        /**
         * @brief This function is called on component creation.
         */
        virtual void onComponentCreated()
        {
        }

        /**
         * @brief This function is called on component activation.
         */
        virtual void onComponentActivated()
        {
        }

        /**
         * @brief This function is called on component deactivation.
         */
        virtual void onComponentDeactivated()
        {
        }

        /**
         * @brief This function is called on component destruction.
         */
        virtual void onComponentDestroyed()
        {
        }

        /**
         * @brief This function is called when a scene containing the component is created from an asset.
         */
        virtual void onAfterComponentRestored()
        {
        }

        // virtual void start()
        // {
        // }
    };

}  // namespace nau::scene
