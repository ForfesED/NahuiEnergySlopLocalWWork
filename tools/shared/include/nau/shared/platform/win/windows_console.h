// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/shared/api.h"

#if defined(_WIN32) || defined(_WIN64)
namespace nau
{
    class SHARED_API WinConsoleStyle
    {
    public:
        void setColor(unsigned char color);
        void reset();
    };
}
#endif