// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/interfaces/animatable.h"
#include "nau/math/transform.h"

namespace nau::animation
{
    class NAU_ANIMATION_EXPORT ITransformAnimatable : public IAnimatable
    {
        NAU_INTERFACE(nau::animation::ITransformAnimatable, IAnimatable)

        virtual void animateTransform(const math::Transform& transform) = 0;
        virtual void animateTranslation(const math::vec3& translation) = 0;
        virtual void animateRotation(const math::quat& rotation) = 0;
        virtual void animateScale(const math::vec3& scale) = 0;
    };

    class NAU_ANIMATION_EXPORT ITransformAndSkewAnimatable : public ITransformAnimatable
    {
        NAU_INTERFACE(nau::animation::ITransformAndSkewAnimatable, ITransformAnimatable)

        virtual void animateSkew(math::vec2 skew) = 0;
    };
}  // namespace nau::animation
