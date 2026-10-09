// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/rtti/rtti_impl.h"

namespace nau::scene
{
    class SceneObject;
}

namespace nau::animation
{
    class IAnimatable;
    class IAnimationPlayer;

    class NAU_ANIMATION_EXPORT IAnimationTarget : public virtual IRefCounted
    {
        NAU_INTERFACE(nau::animation::IAnimationTarget, IRefCounted)

        using Ptr = nau::Ptr<IAnimationTarget>;

        void* getTarget(const rtti::TypeInfo& requestedTarget);
        virtual void* getTarget(const rtti::TypeInfo& requestedTarget, IAnimationPlayer* player);
        virtual scene::SceneObject* getOwner() { return nullptr; }
    };

    /**
     * @brief Provides an interface for an object that can be animated.
     * 
     * Usually it is implemented by a game object component.
     */
    class IAnimatable : public virtual IAnimationTarget
    {
        NAU_INTERFACE(nau::animation::IAnimatable, IAnimationTarget)

        using Ptr = nau::Ptr<IAnimatable>;
    };
} // namespace nau::animation