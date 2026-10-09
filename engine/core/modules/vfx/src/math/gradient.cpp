// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "gradient.h"

namespace nau::vfx::math
{
    nau::math::Color4 interpolateColor(const nau::math::Color4& color1, const nau::math::Color4& color2, float t)
    {
        return nau::math::Color4(nau::math::lerp(color1.r, color2.r, t),
                                 nau::math::lerp(color1.g, color2.g, t),
                                 nau::math::lerp(color1.g, color2.g, t),
                                 nau::math::lerp(color1.g, color2.g, t));
    }

    void Gradient::addStop(float position, const nau::math::Color4& color)
    {
        if (m_gradientStops.size() > MaxPoints)
            return;

        m_gradientStops[position] = color;
    }

    nau::math::Color4 Gradient::getColorAt(float position) const
    {
        if (position <= m_gradientStops.begin()->first)
        {
            return m_gradientStops.begin()->second;
        }
        if (position >= m_gradientStops.rbegin()->first)
        {
            return m_gradientStops.rbegin()->second;
        }

        auto lower = m_gradientStops.lower_bound(position);
        auto upper = lower--;

        float t = (position - lower->first) / (upper->first - lower->first);
        return interpolateColor(lower->second, upper->second, t);
    }
}  // namespace nau::vfx::math