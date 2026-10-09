// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

// #include <util/dag_globDef.h>
#include <nau/dag_ioSys/dag_zlibIo.h>

namespace nau::iosys
{
    void ZlibLoadCB::issueFatal()
    {
        NAU_ASSERT(0 && "restricted by design");
    }
    void ZlibSaveCB::issueFatal()
    {
        NAU_ASSERT(0 && "restricted by design");
    }
}  // namespace nau::iosys