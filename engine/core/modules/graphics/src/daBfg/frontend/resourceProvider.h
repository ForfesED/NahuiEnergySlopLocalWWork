// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <EASTL/variant.h>
#include <EASTL/vector_map.h>
#include "nau/3d/dag_resPtr.h"
#include "nau/math/math.h"

#include "render/daBfg/detail/resNameId.h"
#include "render/daBfg/detail/autoResTypeNameId.h"
#include "render/daBfg/detail/blob.h"
#include "dabfg/id/idIndexedMapping.h"


namespace dabfg
{

using ProvidedResource = eastl::variant<ManagedTexView, ManagedBufView, dabfg::BlobView>;

struct ResourceProvider
{
  eastl::vector_map<ResNameId, ProvidedResource> providedResources;
  eastl::vector_map<ResNameId, ProvidedResource> providedHistoryResources;

  IdIndexedMapping<AutoResTypeNameId, nau::math::IVector2> resolutions;

  void clear()
  {
    providedResources.clear();
    providedHistoryResources.clear();
  }
};

} // namespace dabfg
