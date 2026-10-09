// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/playback/animation.h"

namespace nau::animation
{
    /**
     * @brief Blends multiple animations (used for skeletal animations).
     */
    class NAU_ANIMATION_EXPORT AnimationMixer: public virtual IRefCounted
    {
        NAU_CLASS(nau::animation::AnimationMixer, rtti::RCPolicy::Concurrent, IRefCounted);

    public:

        virtual void blendAnimations(const IAnimatable::Ptr& target) = 0;
        virtual void computeFinalTransforms(const IAnimatable::Ptr& target) = 0;

    };
}  // namespace nau::animation
