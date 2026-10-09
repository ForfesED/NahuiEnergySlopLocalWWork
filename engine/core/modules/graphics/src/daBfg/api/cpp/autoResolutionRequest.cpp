// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "render/daBfg/autoResolutionRequest.h"
#include "dabfg/frontend/internalRegistry.h"


namespace dabfg
{

nau::math::IVector2 AutoResolutionRequest::get() const { return provider->resolutions[autoResTypeId]; }

} // namespace dabfg
