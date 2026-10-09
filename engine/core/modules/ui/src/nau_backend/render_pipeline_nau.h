// Copyright 2024 N-GINN LLC. All rights reserved.
/****************************************************************************
 Copyright (c) 2018-2019 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#pragma once

#include <vector>

#include "nau/3d/dag_drv3d.h"
#include "nau/3d/dag_renderStates.h"
#include "renderer/backend/RenderPipeline.h"
#include "renderer/backend/RenderPipelineDescriptor.h"

DAGOR_CC_BACKEND_BEGIN

class ProgramNau;

/**
 * Set program and blend state.
 */
class RenderPipelineNau : public cocos2d::backend::RenderPipeline
{
public:
    /**
     * @param descriptor Specifies render pipeline descriptor.
     */
    RenderPipelineNau()
    {
        int i = 0;
    };

    virtual void update(const cocos2d::PipelineDescriptor& pipelineDescirptor, const cocos2d::backend::RenderPassDescriptor& renderpassDescriptor) override;

    void apply(shaders::RenderState& renderState);

    cocos2d::backend::RenderPassDescriptor m_renderPassDescriptor;

    ~RenderPipelineNau() override;

private:
    cocos2d::PipelineDescriptor m_pipelineDescriptor;
    Sbuffer* consBuffer = nullptr;
};

DAGOR_CC_BACKEND_END
