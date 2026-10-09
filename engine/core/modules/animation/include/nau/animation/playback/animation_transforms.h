// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/interfaces/animatable_transforms.h"
#include "nau/animation/playback/animation_impl.h"

namespace nau::animation
{
    class NAU_ANIMATION_EXPORT TransformAnimation : public AnimationImpl<math::Transform>
    {
        NAU_CLASS(nau::animation::TransformAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<math::Transform>)   

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    private:
        math::Transform interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    class NAU_ANIMATION_EXPORT TranslationAnimation : public AnimationImpl<math::vec3>
    {
        NAU_CLASS(nau::animation::TranslationAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<math::vec3>)

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    private:
        math::vec3 interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    class NAU_ANIMATION_EXPORT RotationAnimation : public AnimationImpl<math::quat>
    {
        NAU_CLASS(nau::animation::RotationAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<math::quat>)

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    private:
        math::quat interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    class NAU_ANIMATION_EXPORT ScaleAnimation : public AnimationImpl<math::vec3>
    {
        NAU_CLASS(nau::animation::ScaleAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<math::vec3>)

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    private:
        math::vec3 interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    class NAU_ANIMATION_EXPORT SkewAnimation : public AnimationImpl<math::vec2>
    {
        NAU_CLASS(nau::animation::SkewAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<math::vec2>)

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    private:
        math::vec2 interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    enum class TransformAnimationActions : uint8_t
    {
        Translation = NauFlag(0),
        Rotation = NauFlag(1),
        Scale = NauFlag(2)
    };

    NAU_DEFINE_TYPED_FLAG(TransformAnimationActions)
}  // namespace nau::animation
