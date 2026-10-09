// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "normal_state.h"

namespace nau::ui
{

    bool NormalState::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button normal state sprite frame");

            return false;
        }

        return true;
    }

    void NormalState::enter(nau::ui::NauButton* button)
    {
        setupTexture(button);
    }

    void NormalState::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
    {
        if (eventType == nau::ui::EventType::hover)
        {
            button->changeState(nau::ui::UIState::hovered);
        }
    }

    void NormalState::update(nau::ui::NauButton* button) {}

    void NormalState::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui