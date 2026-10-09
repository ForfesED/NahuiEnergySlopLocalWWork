// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/data/frame_event.h"

#include <EASTL/vector.h>

namespace nau::animation
{
    /**
     * @brief Encapsulates per-frame data.
     */
    struct Frame
    {

        /**
         * @brief Frame index.
         */
        int frame;

        /**
         * @brief A collection of events to be triggered at this frame.
         */
        eastl::vector<FrameEvent> events;
    };
}