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


#include "renderer/backend/Device.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/service/service.h"

DAGOR_CC_BACKEND_BEGIN


struct NauDeviceProvider final : public nau::IServiceInitialization,
                                 public cocos2d::backend::IDeviceProvider
{
    NAU_RTTI_CLASS(
      nau::cocos_backend::NauDeviceProvider,
      cocos2d::backend::IDeviceProvider
    );

    virtual cocos2d::backend::Device * GetDevice() override;
};

/**
 * Use to create resoureces.
 */
class DeviceNau : public cocos2d::backend::Device
{
public:
    DeviceNau();
    ~DeviceNau();

    /**
     * New a CommandBuffer object, not auto released.
     * @return A CommandBuffer object.
     */
    virtual cocos2d::backend::CommandBuffer* newCommandBuffer() override;

    /**
     * New a Buffer object, not auto released.
     * @param size Specifies the size in bytes of the buffer object's new data store.
     * @param type Specifies the target buffer object. The symbolic constant must be BufferType::VERTEX or BufferType::INDEX.
     * @param usage Specifies the expected usage pattern of the data store. The symbolic constant must be BufferUsage::STATIC, BufferUsage::DYNAMIC.
     * @return A Buffer object.
     */
    virtual cocos2d::backend::Buffer* newBuffer(std::size_t size, cocos2d::backend::BufferType type, cocos2d::backend::BufferUsage usage) override;

    /**
     * New a TextureBackend object, not auto released.
     * @param descriptor Specifies texture description.
     * @return A TextureBackend object.
     */
    virtual cocos2d::backend::TextureBackend* newTexture(const cocos2d::backend::TextureDescriptor& descriptor) override;

    /**
     * Create an auto released DepthStencilState object.
     * @param descriptor Specifies depth and stencil description.
     * @return An auto release DepthStencilState object.
     */
    virtual cocos2d::backend::DepthStencilState* createDepthStencilState(const cocos2d::backend::DepthStencilDescriptor& descriptor) override;

    /**
     * New a RenderPipeline object, not auto released.
     * @param descriptor Specifies render pipeline description.
     * @return A RenderPipeline object.
     */
    virtual cocos2d::backend::RenderPipeline* newRenderPipeline() override;

    /**
     * Design for metal.
     */
    virtual void setFrameBufferOnly(bool frameBufferOnly) override
    {
    }

    /**
     * New a Program, not auto released.
     * @param vertexShader Specifes this is a vertex shader source.
     * @param fragmentShader Specifes this is a fragment shader source.
     * @return A Program instance.
     */
    virtual cocos2d::backend::Program* newProgram(const std::string& vertexShader, const std::string& fragmentShader) override;

protected:
    /**
     * New a shaderModule, not auto released.
     * @param stage Specifies whether is vertex shader or fragment shader.
     * @param source Specifies shader source.
     * @return A ShaderModule object.
     */
    virtual cocos2d::backend::ShaderModule* newShaderModule(cocos2d::backend::ShaderStage stage, const std::string& source) override
    {
        return nullptr;
    };
};

DAGOR_CC_BACKEND_END
