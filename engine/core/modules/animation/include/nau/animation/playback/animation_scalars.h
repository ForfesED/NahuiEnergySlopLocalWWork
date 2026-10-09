// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/playback/animation_impl.h"
#include "nau/math/transform.h"

namespace nau::animation
{
    class NAU_ANIMATION_EXPORT BoolAnimation : public AnimationImpl<bool>
    {
        NAU_CLASS(nau::animation::BoolAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<bool>)   

    protected:
        virtual void apply(int frame, AnimationState& animationState) const override;
    };

    class NAU_ANIMATION_EXPORT IntegerAnimation : public AnimationImpl<int>
    {
        NAU_CLASS(nau::animation::IntegerAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<int>)   

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    protected:
        int interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    class NAU_ANIMATION_EXPORT FloatAnimation : public AnimationImpl<float>
    {
        NAU_CLASS(nau::animation::FloatAnimation, rtti::RCPolicy::Concurrent, AnimationImpl<float>)   

    public:
        virtual void apply(int frame, AnimationState& animationState) const override;

    protected:
        float interpolate(int targetFrame, const TKeyFrame& from, const TKeyFrame& to, AnimationState& animationState) const;
    };

    class NAU_ANIMATION_EXPORT ScalarParameterAnimatable : public IAnimatable
    {
        NAU_RTTI_CLASS(nau::animation::ScalarParameterAnimatable, IAnimatable)
        
        virtual void animateBool(bool value) { }
        virtual void animateInteger(int value) { }
        virtual void animateFloat(float value) { }
    };
}  // namespace nau::animation
