// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/app/window_manager.h


#pragma once


#include "nau/rtti/rtti_object.h"

namespace nau
{
    struct IPlatformWindow;

    /**
     */
    struct NAU_ABSTRACT_TYPE IWindowManager : virtual IRefCounted
    {
        NAU_INTERFACE(nau::IWindowManager, IRefCounted)

        virtual IPlatformWindow& getActiveWindow() = 0;
    };

}  // namespace nau
