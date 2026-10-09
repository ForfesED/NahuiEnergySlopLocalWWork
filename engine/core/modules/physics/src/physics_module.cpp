// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/module/module.h"
#include "nau/physics/components/rigid_body_component.h"
#include "physics_service.h"

namespace nau
{
    /**
     */
    class PhysicsModule : public DefaultModuleImpl
    {
        string getModuleName() override
        {
            return "nau.physics";
        }

        void initialize() override
        {
            NAU_MODULE_EXPORT_CLASS(physics::PhysicsService);
            NAU_MODULE_EXPORT_CLASS(physics::RigidBodyComponent);
        }
    };
}  // namespace nau

IMPLEMENT_MODULE(nau::PhysicsModule)
