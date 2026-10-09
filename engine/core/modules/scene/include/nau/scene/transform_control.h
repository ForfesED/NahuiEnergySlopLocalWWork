// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/math/transform.h"
#include "nau/meta/class_info.h"
#include "nau/rtti/type_info.h"

namespace nau::scene
{
    /**
     * @brief Provides interface for read-only access to the object transform properties.
     */
    struct TransformProperties
    {
        NAU_TYPEID(nau::scene::TransformProperties)
        
        /**
         * @brief Destructor.
         */
        virtual ~TransformProperties() = default;


        /**
         * @brief Retrieves object world transformation.
         * 
         * @return Object transformation in world coordinates.
         */
        virtual const math::Transform& getWorldTransform() const = 0;

        /**
         * @brief Retrieves object local transformation.
         * 
         * @return Object transformation with respect to its parent.
         */
        virtual const math::Transform& getTransform() const = 0;

        /**
         * @brief Retrieves object local rotation.
         *
         * @return Object rotation with respect to its parent.
         */
        virtual math::quat getRotation() const = 0;

        /**
         * @brief Retrieves object local translation.
         *
         * @return Object translation with respect to its parent.
         */
        virtual math::vec3 getTranslation() const = 0;

        /**
         * @brief Retrieves object local scale.
         * 
         * @return Object scale with respect to its parent.
         */
        virtual math::vec3 getScale() const = 0;
    };

    /**
     * @brief Provides interface for read&write access to the object transform properties.
     */
    struct TransformControl : virtual TransformProperties
    {
        NAU_TYPEID(nau::scene::TransformControl)
        NAU_CLASS_BASE(TransformProperties)

        /**
         * @brief Destructor.
         */
        virtual ~TransformControl() = default;

        /**
         * @brief Sets object transform in world coordinates.
         * 
         * @param [in] transform Transform to assign.
         */
        virtual void setWorldTransform(const math::Transform& transform) = 0;

        /**
         * @brief Sets object transform relative to its parent.
         *
         * @param [in] transform Transform to assign.
         */
        virtual void setTransform(const math::Transform& transform) = 0;

        /**
         * @brief Sets object rotation relative to its parent.
         *
         * @param [in] transform Transform to assign.
         */
        virtual void setRotation(math::quat rotation) = 0;

        /**
         * @brief Sets object translation relative to its parent.
         *
         * @param [in] transform Transform to assign.
         */
        virtual void setTranslation(math::vec3 position) = 0;

        /**
         * @brief Sets object scale relative to its parent.
         *
         * @param [in] transform Transform to assign.
         */
        virtual void setScale(math::vec3 scale) = 0;
    };
}  // namespace nau::scene
