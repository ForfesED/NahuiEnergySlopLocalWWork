// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/math/dag_color.h"
#include "math/gradient.h"

namespace nau::vfx::modfx::settings
{
    struct FxValueGradient
    {
        FxValueGradient() :
            enabled(false),
            gradient(vfx::math::Gradient())
        {
        }

        bool enabled;
        vfx::math::Gradient gradient;
    };

    struct FxColor
    {
        FxColor() :
            enabled(false),
            start_color(nau::math::Color4()),
            end_color(nau::math::Color4()),
            gradient(FxValueGradient())
        {
        }

        bool enabled;

        nau::math::Color4 start_color;
        nau::math::Color4 end_color;

        FxValueGradient gradient;
    };
}  // namespace nau::vfx::modfx::settings