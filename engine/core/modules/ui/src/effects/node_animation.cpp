// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/effects/node_animation.h"

namespace nau::ui
{
    UiNodeAnimator::UiNodeAnimator(nau::ui::Node& node)
        : m_node(node)
    {
    }

    void UiNodeAnimator::animateTransform(const math::Transform& transform)
    {
        animateTranslation(transform.getTranslation());
        animateRotation(transform.getRotation());
        animateScale(transform.getScale());
    }

    void UiNodeAnimator::animateTranslation(const math::vec3& translation)
    {
        m_node.setPosition(math::vec2(translation.getX(), translation.getY()));
    }

    void UiNodeAnimator::animateRotation(const math::quat& rotation)
    {
        math::vec3 euler = rotation.toEuler();
        m_node.setRotation(euler.getZ());
    }

    void UiNodeAnimator::animateScale(const math::vec3& scale)
    {
        m_node.setScale(scale.getX(), scale.getY());
    }

    void UiNodeAnimator::animateSkew(math::vec2 skew)
    {
        m_node.setSkewX(skew.getX());
        m_node.setSkewY(skew.getY());
    }

    void UiNodeAnimator::animateColor(const math::Color3& color)
    {
        auto intColor = e3dcolor(color);
        m_node.setColor({ intColor.r, intColor.g, intColor.b });
    }

    void UiNodeAnimator::animateOpacity(float opacity)
    {
        uint8_t intOpacity = static_cast<uint8_t>(255.f * opacity);
        m_node.setOpacity(intOpacity);
    }

} // namespace nau::ui::widgets