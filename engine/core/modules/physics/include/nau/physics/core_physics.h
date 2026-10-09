// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/physics/physics_world.h"
#include "nau/rtti/type_info.h"
#include "nau/utils/uid.h"

namespace nau::physics
{
    /**
     */
    struct NAU_ABSTRACT_TYPE ICorePhysics
    {
        NAU_TYPEID(nau::physics::ICorePhysics)

        virtual ~ICorePhysics() = default;

        // TODO: this method must be thread safe or called only from physics thread
        virtual nau::Ptr<IPhysicsWorld> findPhysicsWorld(Uid worldUid) = 0;

        inline auto getDefaultPhysicsWorld()
        {
            return findPhysicsWorld(NullUid);
        }
    };
}  // namespace nau::physics
