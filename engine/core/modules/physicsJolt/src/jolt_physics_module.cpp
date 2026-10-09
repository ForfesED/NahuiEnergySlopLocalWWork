// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "jolt_asset_factory.h"
#include "jolt_physics_collision_shapes_factory.h"
#include "nau/module/module.h"
#include "nau/physics/jolt/jolt_physics_world.h"


namespace nau
{
    /**
     */
    class PhysicsJoltModule : public DefaultModuleImpl
    {
        string getModuleName() override
        {
            return "nau.physics.jolt";
        }

        void initialize() override
        {
            NAU_MODULE_EXPORT_CLASS(physics::jolt::JoltPhysicsWorld);
            NAU_MODULE_EXPORT_CLASS(physics::jolt::JoltPhysicsCollisionShapesFactory);
            NAU_MODULE_EXPORT_SERVICE(physics::jolt::JoltAssetFactory);
        }
    };
}  // namespace nau

IMPLEMENT_MODULE(nau::PhysicsJoltModule)
