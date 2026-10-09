// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "easing_storage.h"

namespace nau::vfx::math
{
    float EasingStorage::evaluate(EasingType type, float position)
    {
        float scaledPos = position * (PointsCount - 1);
        int index = static_cast<int>(scaledPos);
        float t = scaledPos - index;

        const auto& points = pointsByType(type);
        float p1 = points[index];
        float p2 = points[index + 1];

        return p1 + t * (p2 - p1);
    }

    const std::array<float, EasingStorage::PointsCount>& EasingStorage::pointsByType(EasingType type)
    {
        switch (type)
        {
            case EasingType::Linear:
                return LinearPoints;
            case EasingType::EaseIn:
                return EaseInPoints;
            case EasingType::EaseOut:
                return EaseOutPoints;
        }
    }
}  // namespace nau::vfx::math
