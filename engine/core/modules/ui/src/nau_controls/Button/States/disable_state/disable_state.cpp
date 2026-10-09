// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "disable_state.h"

namespace nau::ui
{
    bool DisableState::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        if (!tryCreateStateSpriteFrame(data.disableImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button disable state sprite frame");

            return false;
        }

        return true;
    }

    void DisableState::enter(nau::ui::NauButton* button)
    {
        setupTexture(button);
    }

    void DisableState::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType) {}

    void DisableState::update(nau::ui::NauButton* button) {}

    void DisableState::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui