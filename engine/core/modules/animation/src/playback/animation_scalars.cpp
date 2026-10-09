// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/playback/animation_scalars.h"

#include "animation_helper.h"

namespace nau::animation
{
    // Boolean parameter

    void BoolAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = kfFrom->getValue();

            if(auto* target = getAnimatableTarget<ScalarParameterAnimatable>(animationState))
            {
                target->animateBool(value);
            }
        }
    }

    // Integer parameter

    void IntegerAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if(auto* target = getAnimatableTarget<ScalarParameterAnimatable>(animationState))
            {
                target->animateInteger(value);
            }
        }
    }

    int IntegerAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if(animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());

            return lerp(from.getValue(), to.getValue(), t);
        }

        return from.getValue();
    }

    // Float parameter

    void FloatAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if(auto* target = getAnimatableTarget<ScalarParameterAnimatable>(animationState))
            {
                target->animateFloat(value);
            }
        }
    }

    float FloatAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if(animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());

            return lerp(from.getValue(), to.getValue(), t);
        }

        return from.getValue();
    }

}  // namespace nau::animation
