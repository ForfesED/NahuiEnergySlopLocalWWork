// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/interfaces/animatable_graphic_props.h"
#include "nau/animation/interfaces/animatable_transforms.h"

#include "nau/ui/elements/node.h"

namespace nau::ui
{
    class UiNodeAnimator : public animation::ITransformAndSkewAnimatable, public animation::IGraphicPropsAnimatable
    {
        NAU_CLASS(UiNodeAnimator, rtti::RCPolicy::StrictSingleThread, ITransformAndSkewAnimatable, IGraphicPropsAnimatable)

    public:
        UiNodeAnimator(nau::ui::Node& node);

        virtual void animateTransform(const math::Transform& transform) override;
        virtual void animateTranslation(const math::vec3& translation) override;
        virtual void animateRotation(const math::quat& rotation) override;
        virtual void animateScale(const math::vec3& scale) override;
        virtual void animateSkew(math::vec2 skew) override;

        virtual void animateColor(const math::Color3& color) override;
        virtual void animateOpacity(float opacity) override;

    private:
        nau::ui::Node& m_node;
    };
} // namespace nau::ui