// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/playback/animation_impl.h"
#include "nau/animation/playback/animation_scalars.h"
#include "nau/math/dag_color.h"

namespace nau::animation
{
    class NAU_ANIMATION_EXPORT ColorAnimation : public AnimationImpl<math::Color3>
    {
        NAU_CLASS(nau::animation::ColorAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<math::Color3>)

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    private:
        math::Color3 interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    class NAU_ANIMATION_EXPORT OpacityAnimation : public FloatAnimation
    {
        NAU_CLASS(nau::animation::OpacityAnimation, rtti::RCPolicy::Concurrent, FloatAnimation)

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;
    };
}  // namespace nau::animation
