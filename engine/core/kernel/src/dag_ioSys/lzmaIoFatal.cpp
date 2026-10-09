// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include <nau/dag_ioSys/dag_lzmaIo.h>
#include <nau/utils/dag_baseDef.h>

namespace nau::iosys
{
    void LzmaLoadCB::issueFatal()
    {
        NAU_ASSERT(0 && "restricted by design");
    }
}  // namespace nau::iosys