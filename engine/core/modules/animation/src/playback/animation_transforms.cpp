// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/playback/animation_transforms.h"

#include "animation_helper.h"

namespace nau::animation
{
    // Transform matrix

    void TransformAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if(auto* transformTarget = getAnimatableTarget<ITransformAnimatable>(animationState))
            {
                transformTarget->animateTransform(value);
            }
        }
    }

    nau::math::Transform TransformAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if(animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());

            Transform targetTransform = Transform::identity();
            const auto& fromValue = from.getValue();
            const auto& toValue = to.getValue();

            vec3 pos = lerp(t, fromValue.getTranslation(), toValue.getTranslation());
            targetTransform.setTranslation(pos);
            quat rotation = slerp(t, fromValue.getRotation(), toValue.getRotation());
            targetTransform.setRotation(rotation);
            vec3 scale = lerp(t, fromValue.getScale(), toValue.getScale());
            targetTransform.setScale(scale);

            return targetTransform;
        }

        return from.getValue();
    }

    // Translation only

    void TranslationAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if(auto* transformTarget = getAnimatableTarget<ITransformAnimatable>(animationState))
            {
                const auto weightedValue = value * animationState.getFullWeight();

                transformTarget->animateTranslation(weightedValue);
            }
        }
    }

    nau::math::vec3 TranslationAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if(animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());

            return lerp(t, from.getValue(), to.getValue());
        }

        return from.getValue();
    }

    // Rotation only

    void RotationAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if(auto* transformTarget = getAnimatableTarget<ITransformAnimatable>(animationState))
            {
                const auto weightedValue = math::slerp(animationState.getFullWeight(), math::quat::identity(), value);

                transformTarget->animateRotation(weightedValue);
            }
        }
    }

    math::quat RotationAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if(animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());

            return lerp(t, from.getValue(), to.getValue());
        }
        
        return from.getValue();
    }

    // Scale only

    void ScaleAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame *kfFrom = nullptr, *kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if(kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if(auto* transformTarget = getAnimatableTarget<ITransformAnimatable>(animationState))
            {
                const static math::vec3 ones(1.f, 1.f, 1.f);
                const auto weightedValue = lerp(animationState.getFullWeight(), ones, value);

                transformTarget->animateScale(weightedValue);
            }
        }
    }

    math::vec3 ScaleAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if(animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());

            return lerp(t, from.getValue(), to.getValue());
        }

        return from.getValue();
    }

    // Skew

    void SkewAnimation::apply(int frame, AnimationState& animationState) const
    {
        const TKeyFrame* kfFrom = nullptr, * kfTo = nullptr;

        findKeyFrames(*this, frame, animationState, kfFrom, kfTo);

        if (kfFrom && kfTo && animationState.target)
        {
            const auto value = interpolate(frame, *kfFrom, *kfTo, animationState);

            if (auto* transformTarget = getAnimatableTarget<ITransformAndSkewAnimatable>(animationState))
            {
                const auto weightedValue = lerp(animationState.getFullWeight(), math::vec2(.0f, .0f), value);

                transformTarget->animateSkew(weightedValue);
            }
        }
    }

    math::vec2 SkewAnimation::interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const
    {
        if (animationState.interpolationMethod == AnimationInterpolationMethod::Linear)
        {
            using namespace math;

            float t = (targetFrame - from.getFrame()) / (float)(to.getFrame() - from.getFrame());

            return lerp(t, from.getValue(), to.getValue());
        }

        return from.getValue();
    }

}  // namespace nau::animation
