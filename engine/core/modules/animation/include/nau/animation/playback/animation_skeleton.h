// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/playback/animation_mixer.h"
#include "nau/animation/playback/animation_impl.h"
#include "nau/animation/components/skeleton_component.h"
#include <ozz/animation/runtime/animation.h>

namespace nau::animation
{
    class NAU_ANIMATION_EXPORT SkeletalAnimation : public AnimationImpl<ISkeletonAnimatableKeyFrameType>
    {
        NAU_CLASS_(nau::animation::SkeletalAnimation, AnimationImpl<ISkeletonAnimatableKeyFrameType>)   

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

        virtual float getDurationInFrames() const override;

        ozz::animation::Animation ozzAnimation;
    };

     class NAU_ANIMATION_EXPORT SkeletalAnimationMixer : public AnimationMixer
     {
        NAU_CLASS_(nau::animation::SkeletalAnimationMixer, nau::animation::AnimationMixer, IRefCounted);
     public:
         void blendAnimations(const IAnimatable::Ptr& target) override;
         void computeFinalTransforms(const IAnimatable::Ptr& target) override;
     };

}  // namespace nau::animation
