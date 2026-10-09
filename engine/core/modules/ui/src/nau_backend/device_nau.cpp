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


#include "device_nau.h"

#include "buffer_nau.h"
#include "command_buffer_nau.h"
#include "depth_stencil_state_nau.h"
#include "device_info_nau.h"
#include "program_nau.h"
#include "render_pipeline_nau.h"
#include "texture_nau.h"


namespace
{
  static cocos2d::backend::Device* s_instance = nullptr;
}

cocos2d::backend::Device * nau::cocos_backend::NauDeviceProvider::GetDevice()
{
    if (!s_instance)
        s_instance = new (std::nothrow) nau::cocos_backend::DeviceNau();

    return s_instance;
};

DAGOR_CC_BACKEND_BEGIN

using namespace cocos2d::backend;

DeviceNau::DeviceNau()
{
    _deviceInfo = new (std::nothrow) DeviceInfoNau();
    if (!_deviceInfo || _deviceInfo->init() == false)
    {
        delete _deviceInfo;
        _deviceInfo = nullptr;
    }
}

DeviceNau::~DeviceNau()
{
    ProgramCache::destroyInstance();
    delete _deviceInfo;
    _deviceInfo = nullptr;
    s_instance = nullptr;
}

CommandBuffer* DeviceNau::newCommandBuffer()
{
    return new (std::nothrow) CommandBufferNau();
}

Buffer* DeviceNau::newBuffer(std::size_t size, BufferType type, BufferUsage usage)
{
    return new (std::nothrow) BufferNau(size, type, usage);
}

TextureBackend* DeviceNau::newTexture(const TextureDescriptor& descriptor)
{
    switch (descriptor.textureType)
    {
        case TextureType::TEXTURE_2D:
            return new (std::nothrow) Texture2DNau(descriptor);
        case TextureType::TEXTURE_CUBE:
            return new (std::nothrow) TextureCubeNau(descriptor);
        default:
            return nullptr;
    }
}

DepthStencilState* DeviceNau::createDepthStencilState(const DepthStencilDescriptor& descriptor)
{
    auto ret = new (std::nothrow) DepthStencilStateNau(descriptor);
    return ret;
}

RenderPipeline* DeviceNau::newRenderPipeline()
{
    auto ret = new (std::nothrow) RenderPipelineNau();
    ret->autorelease();
    return ret;
}

Program* DeviceNau::newProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    return new (std::nothrow) ProgramNau(vertexShader, fragmentShader);
}

DAGOR_CC_BACKEND_END
