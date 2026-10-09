// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

//#include <math/integer/dag_IPoint2.h>
#include "nau/math/math.h"


struct D3dInitialSettings
{
  nau::math::IVector2 nonaaZbufSize, aaZbufSize;
  nau::math::IVector2 resolution;
  int maxThreads;
  bool vsync;
  bool allowRetinaRes;
  bool useMpGLEngine;
  uint32_t max_genmip_tex_sz;

  D3dInitialSettings(int screen_x, int screen_y);
};