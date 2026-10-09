// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "disable_state_size.h"

namespace nau::ui
{
    bool DisableStateSize::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        m_stateScale = data.disableScale;

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button disable state");

            return false;
        }

        return true;
    }

    void DisableStateSize::enter(nau::ui::NauButton* button)
    {
        setupSize(button);
    }

    void DisableStateSize::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType) {}

    void DisableStateSize::update(nau::ui::NauButton* button) {}

    void DisableStateSize::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui