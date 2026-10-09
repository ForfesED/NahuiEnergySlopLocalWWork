// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>

#include "nau/rtti/ptr.h"
#include "nau/scene/scene.h"
#include "nau/math/dag_color.h"
#include "nau/animation/playback/animation_instance.h"

namespace nau
{
    namespace scene
    {
        struct IScene;
    }
}

namespace nau::ui
{
    /**
     * @brief Encapsulates button animation data used for a specific state.
     */
    struct NauButtonStateAnimationData
    {
        /**
         * @brief Checks whether the animation is valid.
         * 
         * @return `true` if the animation is valid, `false` otherwise.
         */
        operator bool() const
        {
            return (bool)animation;
        }

        Ptr<animation::AnimationInstance> animation;

        /**
         * @brief Indicates whether the animation will be reversed when played on state exit.
         */
        bool bPlayReversedOnExit = false;
    };

    /**
     * @brief Encapsulates button per-state parameters.
     */
    struct NauButtonData
    {
        std::string defaultImageFileName{}; ///< Image to display when the button is in EventType::normal state.
        std::string hoveredImageFileName{}; ///< Image to display when the button is in EventType::hovered state.
        std::string clickedImageFileName{}; ///< Image to display when the button is clicked on.
        std::string disableImageFileName{}; ///< Image to display when the button is in EventType::disabled state.

        math::Color4 defaultColor = {1.0f, 1.0f, 1.0f, 1.0f}; ///< Color to use for the button when it is in EventType::normal state.
        math::Color4 hoveredColor = {1.0f, 1.0f, 1.0f, 1.0f}; ///< Color to use for the button when it is in EventType::hovered state.
        math::Color4 clickedColor = {1.0f, 1.0f, 1.0f, 1.0f}; ///< Color to use for the button when it is clicked on.
        math::Color4 disableColor = {1.0f, 1.0f, 1.0f, 1.0f}; ///< Color to use for the button when it is in EventType::disabled state.

        NauButtonStateAnimationData normalAnimation;   ///< Animation to play when the button transitions to EventType::normal.
        NauButtonStateAnimationData hoveredAnimation;  ///< Animation to play when the button transitions to EventType::hovered.
        NauButtonStateAnimationData clickedAnimation;  ///< Animation to play when the button is clicked.
        NauButtonStateAnimationData disabledAnimation; ///< Animation to play when the button transitions to EventType::disabled.

        float defaultScale = 1.0f; ///< Button scale to set when the button is in EventType::normal state.
        float hoveredScale = 1.0f; ///< Button scale to set when the button is in EventType::hovered state.
        float clickedScale = 1.0f; ///< Button scale to set when the button is clicked.
        float disableScale = 1.0f; ///< Button scale to set when the button is in EventType::disabled state.
    };
}