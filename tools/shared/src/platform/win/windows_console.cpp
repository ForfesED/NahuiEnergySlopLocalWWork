// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/shared/platform/win/windows_console.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <climits>
#define NAU_COLOR_RED 4
#define NAU_COLOR_YELLOW 14
#define NAU_COLOR_WHITE 15
namespace nau
{
    void WinConsoleStyle::setColor(unsigned char color)
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        int colorFlags = 0;
        switch (color)
        {
        case 0 :case 1:
            colorFlags = NAU_COLOR_WHITE;
            break;
        case 2:
            colorFlags = NAU_COLOR_YELLOW;
            break;
        case 3: case 4:
            colorFlags = NAU_COLOR_RED;
            break;
        default:
            colorFlags = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }
        SetConsoleTextAttribute(console, colorFlags);
    }

    void WinConsoleStyle::reset()
    {
        setColor(CHAR_MAX);
    }
}
#endif