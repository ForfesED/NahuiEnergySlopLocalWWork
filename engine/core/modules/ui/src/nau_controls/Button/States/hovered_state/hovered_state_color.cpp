// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "hovered_state_color.h"

namespace nau::ui
{
    bool HoveredStateColor::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        setStateColor(data.hoveredColor);

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button hovered state sprite frame");

            return false;
        }

        return true;
    }

    void HoveredStateColor::enter(nau::ui::NauButton* button)
    {
        setupColor(button);
    }

    void HoveredStateColor::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
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

    void HoveredStateColor::update(nau::ui::NauButton* button) {}

    void HoveredStateColor::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui
