// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/generic/dag_fixedMoveOnlyFunction.h"
#include "dabfg/frontend/internalRegistry.h"
#include "dabfg/id/idIndexedFlags.h"


namespace dabfg
{

using NodeValidCb = nau::FixedMoveOnlyFunction<8, bool(NodeNameId) const>;
using ResValidCb = nau::FixedMoveOnlyFunction<8, bool(ResNameId) const>;

void dump_internal_registry(const InternalRegistry &registry);
void dump_internal_registry(const InternalRegistry &registry, NodeValidCb nodeValid, ResValidCb resourceValid);

} // namespace dabfg
