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


#include "command_buffer_nau.h"

#include <nau/math/dag_color.h>

#include <algorithm>

#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventType.h"
#include "base/ccMacros.h"
#include "buffer_nau.h"
#include "depth_stencil_state_nau.h"
#include "program_nau.h"
#include "render_pipeline_nau.h"
#include "texture_nau.h"
#include "utils_nau.h"

DAGOR_CC_BACKEND_BEGIN

namespace
{
    using namespace cocos2d::backend;

    Texture* getHandler(TextureBackend* texture)
    {
        if (!texture)
        {
            return nullptr;
        }
        switch (texture->getTextureType())
        {
            case TextureType::TEXTURE_2D:
                return dynamic_cast<Texture2DNau*>(texture)->getHandler();
            case TextureType::TEXTURE_CUBE:
                return dynamic_cast<TextureCubeNau*>(texture)->getHandler();
            default:
                NAU_FAILURE();
                return nullptr;
        }
    }

    void applyTexture(TextureBackend* texture, int slot)
    {
        switch (texture->getTextureType())
        {
            case TextureType::TEXTURE_2D:
                dynamic_cast<Texture2DNau*>(texture)->apply(slot);
                break;
            case TextureType::TEXTURE_CUBE:
                dynamic_cast<TextureCubeNau*>(texture)->apply(slot);
                break;
            default:
                NAU_FAILURE();
                return;
        }
    }
}  // namespace

CommandBufferNau::CommandBufferNau()
{
}

CommandBufferNau::~CommandBufferNau()
{
    CC_SAFE_RELEASE_NULL(_depthStencilStateGL);
    cleanResources();
}

void CommandBufferNau::beginFrame(BaseTexture* backBuffer)
{
    m_backBuffer = backBuffer;
}

void CommandBufferNau::beginRenderPass(const RenderPassDescriptor& descirptor)
{
    applyRenderPassDescriptor(descirptor);
}

inline float fromOpenGLtoDx12Depth(float depth)
{
    // OpenGL depth: [-1; 1]
    // DX depth: [0; 1]
    return (depth + 1)/2;
}

void CommandBufferNau::applyRenderPassDescriptor(const RenderPassDescriptor& descirptor)
{
    bool useColorAttachmentExternal = descirptor.needColorAttachment && descirptor.colorAttachmentsTexture[0];
    bool useDepthAttachmentExternal = descirptor.depthTestEnabled && descirptor.depthAttachmentTexture;
    bool useStencilAttachmentExternal = descirptor.stencilTestEnabled && descirptor.stencilAttachmentTexture;
    bool useGeneratedFBO = false;

    if (useColorAttachmentExternal)
    {
        for (int i = 0; i < MAX_COLOR_ATTCHMENT; ++i)
        {
            d3d::set_render_target(i, getHandler(descirptor.colorAttachmentsTexture[i]), 0);
        }
    }

    if (descirptor.colorAttachmentsTexture[0] == nullptr)
    {
        d3d::set_render_target(m_backBuffer, 0);
    }

    if ((descirptor.depthTestEnabled || descirptor.stencilTestEnabled) && descirptor.depthAttachmentTexture == nullptr)
    {
        updateDepthTexture();
        d3d::set_depth(_depthTarget, 0, DepthAccess::RW);
    }

    if (useDepthAttachmentExternal || useStencilAttachmentExternal)
    {
        d3d::set_depth(getHandler(descirptor.depthAttachmentTexture), 0, DepthAccess::RW);
    }

    // set clear, depth and stencil
    int clearMask = 0;
    if (descirptor.needClearDepth)
    {
        clearMask |= CLEAR_ZBUFFER;
    }
    if (descirptor.needClearStencil)
    {
        clearMask |= CLEAR_STENCIL;
    }

    if (clearMask)
    {
        d3d::clearview(clearMask, e3dcolor(nau::math::Color4(descirptor.clearColorValue.data())), fromOpenGLtoDx12Depth(descirptor.clearDepthValue), descirptor.clearStencilValue);
    }
}

void CommandBufferNau::setRenderPipeline(RenderPipeline* renderPipeline)
{
    NAU_ASSERT(renderPipeline != nullptr);
    if (renderPipeline == nullptr)
        return;

    RenderPipelineNau* rp = static_cast<RenderPipelineNau*>(renderPipeline);
    rp->retain();
    CC_SAFE_RELEASE(_renderPipeline);
    _renderPipeline = rp;
}

void CommandBufferNau::setViewport(int x, int y, unsigned int w, unsigned int h)
{
    _viewPort.x = x;
    _viewPort.y = y;
    _viewPort.w = w;
    _viewPort.h = h;
}

void CommandBufferNau::setCullMode(CullMode mode)
{
    _cullMode = mode;
}

void CommandBufferNau::setWinding(Winding winding)
{
    _winding = winding;
}

void CommandBufferNau::setIndexBuffer(Buffer* buffer)
{
    NAU_ASSERT(buffer != nullptr);
    if (buffer == nullptr)
        return;

    buffer->retain();
    CC_SAFE_RELEASE(_indexBuffer);
    _indexBuffer = static_cast<BufferNau*>(buffer);
}

void CommandBufferNau::setVertexBuffer(Buffer* buffer)
{
    assert(buffer != nullptr);
    if (buffer == nullptr || _vertexBuffer == buffer)
        return;

    buffer->retain();
    _vertexBuffer = static_cast<BufferNau*>(buffer);
}

void CommandBufferNau::setProgramState(ProgramState* programState)
{
    CC_SAFE_RETAIN(programState);
    CC_SAFE_RELEASE(_programState);
    _programState = programState;
}

void CommandBufferNau::drawArrays(PrimitiveType primitiveType, std::size_t start, std::size_t count)
{
    prepareDrawing();
    d3d::draw(cocos_utils::toNauPrimitiveType(primitiveType), start, cocos_utils::toNauPrimitiveCountFromVertexCount(count, primitiveType));
    cleanResources();
}

void CommandBufferNau::drawElements(PrimitiveType primitiveType, IndexFormat indexType, std::size_t count, std::size_t offset)
{
    NAU_ASSERT(indexType != IndexFormat::U_INT, "int32 indexes are unsupported. It should be part of buffers description.");

    prepareDrawing();
    d3d::setind(_indexBuffer->getHandler());
    d3d::drawind(cocos_utils::toNauPrimitiveType(primitiveType), offset / sizeof(uint16_t), cocos_utils::toNauPrimitiveCountFromVertexCount(count, primitiveType), 0);
    cleanResources();
}

void CommandBufferNau::endRenderPass()
{
}

void CommandBufferNau::endFrame()
{
}

void CommandBufferNau::setDepthStencilState(DepthStencilState* depthStencilState)
{
    CC_SAFE_RELEASE_NULL(_depthStencilStateGL);

    if (depthStencilState)
    {
        _depthStencilStateGL = static_cast<DepthStencilStateNau*>(depthStencilState);
    }
}

void CommandBufferNau::prepareDrawing()
{
    bindVertexBuffer();
    bindIndexBuffer();

    shaders::RenderState rendState;
    rendState.cull = cocos_utils::toNauCullMode(_cullMode);

    // Set depth/stencil state.
    if (_depthStencilStateGL)
    {
        _depthStencilStateGL->apply(rendState);
        rendState.stencilRef = _stencilReferenceValueFront;
    }
    else
    {
        rendState.stencil.func = 0;
        rendState.ztest = 0;
    }

    _renderPipeline->apply(rendState);

    applyRenderPassDescriptor(_renderPipeline->m_renderPassDescriptor);

    auto it = std::find_if(cachedRS.begin(), cachedRS.end(), [&rendState](const auto& item)
    {
        return item.first == rendState;
    });

    if (it == cachedRS.end())
    {
        auto renderState = d3d::create_render_state(rendState);
        cachedRS.push_back({rendState, renderState});
        d3d::set_render_state(renderState);
    }
    else
    {
        d3d::set_render_state(it->second);
    }

    d3d::setview(_viewPort.x, _viewPort.y, _viewPort.w, _viewPort.h, 0, 1);
}

void CommandBufferNau::bindVertexBuffer() const
{
    // Bind vertex buffers and set the attributes.
    auto vertexLayout = _programState->getVertexLayout();

    if (!vertexLayout->isValid())
        return;

    d3d::setvsrc_ex(0, _vertexBuffer->getHandler(), 0, vertexLayout->getStride());
}

void CommandBufferNau::bindIndexBuffer() const
{
    if (_indexBuffer)
    {
        d3d::setind(_indexBuffer->getHandler());
    }
    else
    {
        d3d::setind(nullptr);
    }
}

void CommandBufferNau::cleanResources()
{
    CC_SAFE_RELEASE_NULL(_indexBuffer);
    CC_SAFE_RELEASE_NULL(_programState);
    CC_SAFE_RELEASE_NULL(_vertexBuffer);
}

void CommandBufferNau::setLineWidth(float lineWidth)
{
    // TODO: Special geometry shader for this case
}

void CommandBufferNau::setScissorRect(bool isEnabled, float x, float y, float width, float height)
{
    if (isEnabled)
    {
        d3d::setscissor(x, y, width, height);
    }
    else
    {
        d3d::setscissor(_viewPort.x, _viewPort.y, _viewPort.w, _viewPort.h);
    }
}

void CommandBufferNau::captureScreen(std::function<void(const unsigned char*, int, int)> callback)
{
    ::TextureInfo info;
    d3d::get_backbuffer_tex()->getinfo(info, 0);

    NAU_ASSERT((info.cflg & TEXFMT_MASK) == TEXFMT_R8G8B8A8);

    const unsigned char* data = new unsigned char[info.w * info.h * sizeof(math::E3DCOLOR)];
    callback(data, info.w, info.h);
    delete[] data;

    // TODO: using render thread and optimal API.
}

void CommandBufferNau::updateDepthTexture()
{
    ::TextureInfo info;
    d3d::get_backbuffer_tex()->getinfo(info, 0);

    if ((_bbSize != math::IVector2{info.w, info.h}) || (_depthTarget == nullptr))
    {
        _bbSize = math::IVector2{info.w, info.h};
        if (_depthTarget)
        {
            _depthTarget->destroy();
        }

        auto flags = TEXFMT_DEPTH24 | TEXCF_CLEAR_ON_CREATE | TEXCF_RTARGET;

        _depthTarget = d3d::create_tex(nullptr, info.w, info.h, flags, 1);
    }
}

DAGOR_CC_BACKEND_END
