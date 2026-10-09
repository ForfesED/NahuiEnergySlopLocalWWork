// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/playback/animation_graphic_props.h"

#include "nau/animation/interfaces/animatable_graphic_props.h"
#include "animation_helper.h"

namespace nau::animation
{
    // color

    void ColorAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if(auto* target = getAnimatableTarget<IGraphicPropsAnimatable>(animationState))
            {
                target->animateColor(value);
            }
        }
    }

    math::Color3 ColorAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if (animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());
            math::vec3 c0{ from.getValue().r, from.getValue().g, from.getValue().b };
            math::vec3 c1{ to.getValue().r, to.getValue().g, to.getValue().b };

            math::vec3 r = lerp(t, c0, c1);
            return { r.getX(), r.getY(), r.getZ() };
        }
        
        NAU_CONDITION_LOG(animationState.interpolationMethod != AnimationInterpolationMethod::Step, ::nau::diag::LogLevel::Debug, "ColorAnimation is not applying: unsupported interpolation method {}", fmt::underlying(animationState.interpolationMethod));

        return from.getValue();
    }

    // opacity

    void OpacityAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame* kfFrom = nullptr, * kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if (kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if (auto* target = getAnimatableTarget<IGraphicPropsAnimatable>(animationState))
            {
                target->animateOpacity(value);
            }
        }
    }

}  // namespace nau::animation
