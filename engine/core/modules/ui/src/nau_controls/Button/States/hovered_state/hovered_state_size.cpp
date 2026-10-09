// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "hovered_state_size.h"

namespace nau::ui
{

    bool HoveredStateSize::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        m_stateScale = data.hoveredScale;

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button hovered state");

            return false;
        }

        return true;
    }

    void HoveredStateSize::enter(nau::ui::NauButton* button)
    {
        setupSize(button);
    }

    void HoveredStateSize::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
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

    void HoveredStateSize::update(nau::ui::NauButton* button) {}

    void HoveredStateSize::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui