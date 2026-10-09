// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/util/common.h"
#include "nau/osApiWrappers/basePath.h"


eastl::allocator* midmem = new eastl::allocator();

eastl::allocator* inimem = new eastl::allocator();

eastl::allocator* strmem = new eastl::allocator();

eastl::allocator* tmpmem = new eastl::allocator();

eastl::allocator* globmem = new eastl::allocator();
