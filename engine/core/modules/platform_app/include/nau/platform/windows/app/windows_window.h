// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/platform/windows/app/windows_window.h


#pragma once

#include "nau/app/platform_window.h"
#include "nau/platform/windows/windows_headers.h"


namespace nau
{
    struct IWindowsWindow : IPlatformWindow
    {
        NAU_INTERFACE(nau::IWindowsWindow, IPlatformWindow)

        virtual HWND getWindowHandle() const = 0;
    };

}  // namespace nau
