// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "normal_state_color.h"

namespace nau::ui
{

    bool NormalStateColor::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        setStateColor(data.defaultColor);

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button default state sprite frame");

            return false;
        }

        return true;
    }

    void NormalStateColor::enter(nau::ui::NauButton* button)
    {
        setupColor(button);
    }

    void NormalStateColor::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
    {
        if (eventType == nau::ui::EventType::hover)
        {
            button->changeState(nau::ui::UIState::hovered);
        }
    }

    void NormalStateColor::update(nau::ui::NauButton* button) {}

    void NormalStateColor::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui
