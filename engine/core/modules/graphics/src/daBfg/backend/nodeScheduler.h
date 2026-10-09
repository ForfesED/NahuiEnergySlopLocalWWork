// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <ska_hash_map/flat_hash_map2.hpp>
#include <EASTL/span.h>
#include "nau/memory/eastl_aliases.h"

#include "dabfg/common/graphDumper.h"
#include "dabfg/backend/intermediateRepresentation.h"
#include "dabfg/id/idIndexedFlags.h"


namespace dabfg
{

class NodeScheduler
{
public:
  NodeScheduler(IGraphDumper &dumper) : graphDumper{dumper} {}

  // old index -> new index mapping
  using NodePermutation = IdIndexedMapping<intermediate::NodeIndex, intermediate::NodeIndex, nau::EastlFrameAllocator>;
  NodePermutation schedule(const intermediate::Graph &graph);

private:
  IGraphDumper &graphDumper;
};

} // namespace dabfg
