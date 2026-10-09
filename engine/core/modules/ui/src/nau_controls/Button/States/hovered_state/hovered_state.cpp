// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "hovered_state.h"

namespace nau::ui
{

    bool HoveredState::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        if (!tryCreateStateSpriteFrame(data.hoveredImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button hovered state sprite frame");

            return false;
        }

        return true;
    }

    void HoveredState::enter(nau::ui::NauButton* button)
    {
        setupTexture(button);
    }

    void HoveredState::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
    {
        if (eventType == nau::ui::EventType::press)
        {
            button->changeState(nau::ui::UIState::pressed);
        }
        if (eventType == nau::ui::EventType::leave)
        {
            button->changeState(nau::ui::UIState::normal);
        }
    }

    void HoveredState::update(nau::ui::NauButton* button) {}

    void HoveredState::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui