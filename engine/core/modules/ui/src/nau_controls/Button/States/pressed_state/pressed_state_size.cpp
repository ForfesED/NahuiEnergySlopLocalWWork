// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "pressed_state_size.h"

namespace nau::ui
{

    bool PressedStateSize::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        m_stateScale = data.clickedScale;

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button clicked state");

            return false;
        }

        return true;
    }

    void PressedStateSize::enter(nau::ui::NauButton* button)
    {
        setupSize(button);
    }

    void PressedStateSize::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
    {
        if (eventType == nau::ui::EventType::release)
        {
            button->changeState(nau::ui::UIState::hovered);
        }
    }

    void PressedStateSize::update(nau::ui::NauButton* button) {}

    void PressedStateSize::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui