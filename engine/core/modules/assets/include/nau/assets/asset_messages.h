// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include "nau/messaging/messaging.h"

namespace nau
{
    /**
     */
    NAU_DECLARE_MESSAGE(AssetLoaded, "Nau.Assets.AssetLoaded", uint64_t);

    /**
     */
    NAU_DECLARE_MESSAGE(AssetUnloaded, "Nau.Assets.AssetUnloaded", uint64_t);
}  // namespace nau
