// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <EASTL/type_traits.h>
#include <stdint.h>


// TODO: change to dabfg::detail
namespace dabfg
{

enum class NodeNameId : uint16_t
{
  Invalid = static_cast<eastl::underlying_type_t<NodeNameId>>(-1)
};

} // namespace dabfg
