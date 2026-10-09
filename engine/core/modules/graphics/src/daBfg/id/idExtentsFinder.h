// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <EASTL/type_traits.h>
#include <EASTL/algorithm.h>


template <class EnumType>
class IdExtentsFinder
{
  using Underlying = eastl::underlying_type_t<EnumType>;

public:
  void update(EnumType value) { currentMax_ = eastl::max(currentMax_, eastl::to_underlying(value)); }

  Underlying get() const { return currentMax_ + 1; }

private:
  Underlying currentMax_{0};
};
