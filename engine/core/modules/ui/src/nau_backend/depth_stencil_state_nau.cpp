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


#include "depth_stencil_state_nau.h"

#include "base/ccMacros.h"
#include "utils_nau.h"

DAGOR_CC_BACKEND_BEGIN

DepthStencilStateNau::DepthStencilStateNau(const cocos2d::backend::DepthStencilDescriptor& descriptor) :
    DepthStencilState(descriptor)
{
}

void DepthStencilStateNau::apply(shaders::RenderState& renderState) const
{
    renderState.ztest = _depthStencilInfo.depthTestEnabled;
    renderState.zwrite = _depthStencilInfo.depthWriteEnabled;
    renderState.zFunc  = cocos_utils::toNauCompareFunction(_depthStencilInfo.depthCompareFunction);

    //TODO: add to Nau renderer back face stencil
    renderState.stencil.func = cocos_utils::toNauStencilOperation(_depthStencilInfo.frontFaceStencil.depthStencilPassOperation);
    renderState.stencil.zFail = cocos_utils::toNauStencilOperation(_depthStencilInfo.frontFaceStencil.depthFailureOperation);
    renderState.stencil.fail = cocos_utils::toNauStencilOperation(_depthStencilInfo.frontFaceStencil.stencilFailureOperation);
    renderState.stencil.pass = cocos_utils::toNauStencilOperation(_depthStencilInfo.frontFaceStencil.depthStencilPassOperation);
    renderState.stencil.func = cocos_utils::toNauCompareFunction(_depthStencilInfo.frontFaceStencil.stencilCompareFunction);
    renderState.stencil.readMask = _depthStencilInfo.frontFaceStencil.readMask;
    renderState.stencil.writeMask = _depthStencilInfo.frontFaceStencil.writeMask;

}

DAGOR_CC_BACKEND_END
