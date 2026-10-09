// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/samples/components/rotator.h"

namespace nau::sample
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(MyRotator)

    void MyRotator::updateComponent(float dt)
    {
        math::Transform transform = getParentObject().getTransform();
        const float angle = dt * m_speedFactor;

        if (m_axis == Axis::X)
        {
            transform.addRotation(math::quat::rotationX(angle));
        }
        else if (m_axis == Axis::Y)
        {
            transform.addRotation(math::quat::rotationY(angle));
        }
        else
        {
            transform.addRotation(math::quat::rotationZ(angle));
        }
        getParentObject().setTransform(transform);
    }

    void MyRotator::setRotationAxis(Axis axis)
    {
        value_changes_scope;

        m_axis = axis;
    }

    void MyRotator::setSpeedFactor(float factor)
    {
        value_changes_scope;

        m_speedFactor = factor;
    }
}  // namespace nau::sample
