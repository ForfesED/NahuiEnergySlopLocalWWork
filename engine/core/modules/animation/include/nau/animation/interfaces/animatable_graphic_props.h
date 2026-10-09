// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/interfaces/animatable.h"
#include "nau/math/dag_color.h"
#include "nau/math/math.h"

namespace nau::animation
{
    class NAU_ANIMATION_EXPORT IGraphicPropsAnimatable : public IAnimatable
    {
        NAU_INTERFACE(nau::animation::IGraphicPropsAnimatable, IAnimatable)

        virtual void animateColor(const math::Color3& color) = 0;
        virtual void animateOpacity(float opacity) = 0;
    };
}  // namespace nau::animation
