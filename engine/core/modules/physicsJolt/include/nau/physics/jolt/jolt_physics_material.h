// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/physics/physics_defines.h"
#include "nau/physics/physics_material.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Collision/PhysicsMaterial.h>

#include <EASTL/optional.h>

namespace nau::physics::jolt
{
    /**
     * @brief Jolt material class wrapper. 
     */
    class JoltPhysicsMaterial : public JPH::PhysicsMaterial
    {
    public:

        /**
         * @brief Retrieves the name of the material.
         * 
         * @return Material name.
         */
        const char* GetDebugName() const override;

        /**
         * @brief Retrieves native engine material this instance is associated with.
         * 
         * @return A pointer to engine native material instance.
         */
        const nau::physics::IPhysicsMaterial::Ptr& engineMaterial() const;

        /**
         * @brief Associates this instance to a native engine material.
         * 
         * @param [in] engineMaterial   A pointer to the engine material instance.
         */
        void setEngineMaterial(nau::physics::IPhysicsMaterial::Ptr engineMaterial);

    private:

        /**
         * @brief Native engine material this instance is associated with.
         */
        nau::physics::IPhysicsMaterial::Ptr m_engineMaterial;
    };

    /**
     * @brief Implements nau::physics::IPhysicsMaterial interface utilizing Jolt physics engine.
     */
    class NauJoltPhysicsMaterialImpl : public nau::physics::IPhysicsMaterial
    {
        NAU_CLASS(nau::physics::jolt::NauJoltPhysicsMaterialImpl, rtti::RCPolicy::Concurrent, nau::physics::IPhysicsMaterial)

        NauJoltPhysicsMaterialImpl(eastl::string_view name,
            eastl::optional<TFloat> friction = eastl::nullopt, eastl::optional<TFloat> restitution = eastl::nullopt);
        
        /**
         * @brief Retrieves the name of the material.
         *
         * @return Material name.
         */
        eastl::string_view getName() const override;

        /**
         * @brief Retrieves the friction value.
         *
         * @return Friction value. Empty value might be returned in case this material doesn't override the body default friction.
         *
         * See m_friction for more information.
         */
        eastl::optional<TFloat> getFriction() const override;

        /**
         * @brief Retrieves the restitution value.
         *
         * @return Restitution value. Empty value might be returned in case this material doesn't override the body default restitution.
         *
         * See m_restitution for more information.
         */
        eastl::optional<TFloat> getRestitution() const override;

        /**
         * @brief Retrieves the Jolt material ths object is associated with.
         * 
         * @return A pointer to the Jolt material instance.
         */
        JoltPhysicsMaterial* joltMaterial() const;

    private:

        /**
         * @brief Name of the material.
         */
        eastl::string m_name;

        /**
         * @brief Degree of how the body resists being dragged. 
         * 
         * It has to be between 0.0 (no friction) and 1.0 (the body will stick to the surface and stay immobile).
         */
        eastl::optional<TFloat> m_friction;

        /**
         * @brief Degree of body tougheness on collision. 
         * 
         * It has to be between 0.0 (completely inelastic collision response) and 1.0 (completely elastic collision response).
         */
        eastl::optional<TFloat> m_restitution;

        /**
         * @brief A pointer to the Jolt material ths object is associated with.
         */
        JoltPhysicsMaterial* m_joltMaterial = nullptr;
    };

}  // namespace nau::physics
