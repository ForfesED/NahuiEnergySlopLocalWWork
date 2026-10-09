// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/ui/elements/layer.h"


namespace nau::ui
{

Layer* Layer::create()
{
    Layer* ret = new (std::nothrow) Layer();

    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}


}
