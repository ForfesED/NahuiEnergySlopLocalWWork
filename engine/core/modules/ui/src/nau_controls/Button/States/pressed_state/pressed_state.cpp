// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "pressed_state.h"

namespace nau::ui
{

    bool PressedState::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        if (!tryCreateStateSpriteFrame(data.clickedImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button pressed state sprite frame");

            return false;
        }

        return true;
    }

    void PressedState::enter(nau::ui::NauButton* button)
    {
        setupTexture(button);
    }

    void PressedState::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
    {
        if (eventType == nau::ui::EventType::release)
        {
            button->changeState(nau::ui::UIState::hovered);
        }
    }

    void PressedState::update(nau::ui::NauButton* button) {}

    void PressedState::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui