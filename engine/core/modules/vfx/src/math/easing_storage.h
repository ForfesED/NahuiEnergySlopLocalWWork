// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "easing_type.h"

namespace nau::vfx::math
{
    class EasingStorage
    {
    public:
        static float evaluate(EasingType type, float position);

    private:
        static constexpr int PointsCount = 10;

    private:
        static const std::array<float, PointsCount>& pointsByType(EasingType type);

        static constexpr std::array<float, PointsCount> LinearPoints = {
            {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 1.0f}
        };

        static constexpr std::array<float, PointsCount> EaseInPoints = {
            {0.0f, 0.01f, 0.04f, 0.09f, 0.16f, 0.25f, 0.36f, 0.49f, 0.64f, 1.0f}
        };

        static constexpr std::array<float, PointsCount> EaseOutPoints = {
            {0.0f, 0.36f, 0.49f, 0.64f, 0.75f, 0.84f, 0.91f, 0.96f, 0.99f, 1.0f}
        };
    };
}  // namespace nau::vfx::math