// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "ui_render_view.h"
#include "cocos/renderer/backend/Device.h"
#include "CCRenderView.h"
#include "CCPlatformMacros.h"

namespace nau::ui
{
    cocos2d::RenderView* UIRenderView::create(float width, float height)
    {
        UIRenderView* ret = new (std::nothrow) UIRenderView();
        if (ret)
        {
            ret->setFrameSize(width, height);
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    UIRenderView::~UIRenderView()
    {
        cocos2d::backend::Device* device = cocos2d::backend::Device::getInstance();
        CC_SAFE_RELEASE(device);
    }

    void UIRenderView::end()
    {
        release();
    }
}
