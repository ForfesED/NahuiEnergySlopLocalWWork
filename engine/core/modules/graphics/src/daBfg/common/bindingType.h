// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <stdint.h>

namespace dabfg
{

enum class BindingType : uint8_t
{
  ShaderVar,
  ViewMatrix,
  ProjMatrix,
  Invalid,
};

}
