// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/interfaces/animatable.h"

namespace nau::animation
{
    void* IAnimationTarget::getTarget(const rtti::TypeInfo& requestedTarget)
    {
        return getTarget(requestedTarget, nullptr);
    }
    void* IAnimationTarget::getTarget(const rtti::TypeInfo& requestedTarget, IAnimationPlayer* player)
    {
        return as(requestedTarget);
    }

}  // namespace nau::animation
