// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "pressed_state_color.h"

namespace nau::ui
{

    bool PressedStateColor::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        setStateColor(data.clickedColor);

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button pressed state sprite frame");

            return false;
        }

        return true;
    }

    void PressedStateColor::enter(nau::ui::NauButton* button)
    {
        setupColor(button);
    }

    void PressedStateColor::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
    {
        if (eventType == nau::ui::EventType::release)
        {
            button->changeState(nau::ui::UIState::hovered);
        }
    }

    void PressedStateColor::update(nau::ui::NauButton* button) {}

    void PressedStateColor::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui