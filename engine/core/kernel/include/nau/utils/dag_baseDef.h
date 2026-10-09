// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once

#include "nau/core_defines.h"

#if defined(__GNUC__) || defined(__SNC__)
typedef long long __int64;
#endif


#define MAKE4C(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

#define _MAKE4C(x) MAKE4C((int(x) >> 24) & 0xFF, (int(x) >> 16) & 0xFF, (int(x) >> 8) & 0xFF, int(x) & 0xFF)

// dump four-cc code for pattern %c%c%c%c - for debug output
#define DUMP4C(x)  char((x)&0xFF), char(((x) >> 8) & 0xFF), char(((x) >> 16) & 0xFF), char(((x) >> 24) & 0xFF)

// dump four-cc code for pattern %c%c%c%c (0 is replaced with ' ')
#define _DUMP4C(x)                                                                            \
  ((x)&0xFF) ? char(((x)&0xFF)) : ' ', (((x) >> 8) & 0xFF) ? char((((x) >> 8) & 0xFF)) : ' ', \
    (((x) >> 16) & 0xFF) ? char((((x) >> 16) & 0xFF)) : ' ', (((x) >> 24) & 0xFF) ? char((((x) >> 24) & 0xFF)) : ' '


// We might want to have NAU_MAX_PATH with different semantics in parallel
// for a while, for that reason DAGOR_ prefix is kept. Used in ioSys and
// soApiWrappers, which are also imported from Dagor
#if NAU_PLATFORM_WIN32 | _TARGET_XBOX | _TARGET_C1 | _TARGET_C2
static constexpr int DAGOR_MAX_PATH = 260;
#else
static constexpr int DAGOR_MAX_PATH = 516;
#endif

#if !(NAU_PLATFORM_WIN32 | _TARGET_XBOX)
static constexpr int MAX_PATH = DAGOR_MAX_PATH;
#endif
