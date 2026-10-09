// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once
#include <stdint.h>

enum class StereoIndex
{
  Mono,
  Left,
  Right,
  Bounding = Mono,
};

constexpr uint32_t operator*(StereoIndex index) noexcept { return static_cast<uint32_t>(index); }
