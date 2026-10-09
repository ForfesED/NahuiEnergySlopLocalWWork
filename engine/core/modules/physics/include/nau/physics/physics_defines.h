// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <cstdint>

#include "nau/math/math.h"
#include "nau/utils/enum/enum_reflection.h"

namespace nau::physics
{
#ifdef MATH_USE_DOUBLE_PRECISION
    using TFloat = double;
#else
    using TFloat = float;
#endif

    /**
     * @brief Type of the collision channel.
     *
     * Collision channels are used to group bodies and modify their behavior on contact.
     *
     * You can allow or forbid contact between two channels. If the contact between two channels is not allowed, their bodies to not collide.
     * You can allow and forbid contacts between channels by calling nau::physics::IPhysicsWorld::setChannelsCollidable.
     * You can attach a channel to a body by calling nau::physics::IPhysicsBody::setCollisionChannel.
     */
    using CollisionChannel = std::uint16_t;

    /**
     * @brief Motion type of a physical body.
     */
    NAU_DEFINE_ENUM_(MotionType,
                     Static,   /** < Can't be moved by a physical influence (i.e. force, momentum or impulse). */
                     Dynamic,  /** < Can be moved by a physical influence (i.e. force, momentum or impulse). */
                     Kinematic /** < Can be moved by manually applied velocities. */
    )

}  // namespace nau::physics
