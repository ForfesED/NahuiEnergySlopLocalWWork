// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/util/dag_generationRefId.h"

namespace shaders
{
struct RenderState;
class RenderStateIdDummy
{};
using RenderStateId = GenerationRefId<8, RenderStateIdDummy>; // weak reference

class DriverRenderStateIdDummy
{};
using DriverRenderStateId = GenerationRefId<8, DriverRenderStateIdDummy>; // weak reference
} // namespace shaders
