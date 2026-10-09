// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "disable_state_color.h"

namespace nau::ui
{

    bool DisableStateColor::initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data)
    {
        setStateColor(data.disableColor);

        if (!tryCreateStateSpriteFrame(data.defaultImageFileName))
        {
            NAU_LOG_ERROR("Failed to initialize button disable state sprite frame");

            return false;
        }

        return true;
    }

    void DisableStateColor::enter(nau::ui::NauButton* button)
    {
        setupColor(button);
    }

    void DisableStateColor::handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType) {}

    void DisableStateColor::update(nau::ui::NauButton* button) {}

    void DisableStateColor::exit(nau::ui::NauButton* button) {}

} // namespace nau::ui
