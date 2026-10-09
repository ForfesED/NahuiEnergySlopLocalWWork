// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#if _TARGET_D3D_MULTI
#define d3d d3d_multi_dx12
#undef _TARGET_D3D_MULTI
// stub needs to know if it was imported as part of multi or not
#define _TARGET_WAS_MULTI 1
#endif
