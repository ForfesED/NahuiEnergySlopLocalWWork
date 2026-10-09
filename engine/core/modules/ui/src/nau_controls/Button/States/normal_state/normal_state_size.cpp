// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "normal_state_size.h"

namespace nau::ui
{

    bool NormalStateSize::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        m_stateScale = data.defaultScale;

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button default state");

            return false;
        }

        return true;
    }

    void NormalStateSize::enter(nau::ui::NauButton* button)
    {
        setupSize(button);
    }

    void NormalStateSize::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType)
    {
        if (eventType == nau::ui::EventType::hover)
        {
            button->changeState(nau::ui::UIState::hovered);
        }
    }

    void NormalStateSize::update(nau::ui::NauButton* button) {}

    void NormalStateSize::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui