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


#include "buffer_nau.h"

#include <cassert>

#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventType.h"
#include "base/ccMacros.h"

DAGOR_CC_BACKEND_BEGIN

namespace
{
    SBCF inline toNauUsage(const cocos2d::backend::BufferUsage& usage)
    {
        switch (usage)
        {
            case cocos2d::backend::BufferUsage::STATIC:
                return SBCF(0);
            case cocos2d::backend::BufferUsage::DYNAMIC:
                return SBCF_DYNAMIC;
            default:
                return SBCF_DYNAMIC;
        }
    }
    inline SBCF toNauType(const cocos2d::backend::BufferType& type)
    {
        switch (type)
        {
            case cocos2d::backend::BufferType::VERTEX:
                return SBCF_BIND_VERTEX;
            case cocos2d::backend::BufferType::INDEX:
                return SBCF_BIND_INDEX;
            default:
                return SBCF(0);
        }
    }
    inline const char8_t* toNauBufferNameType(const cocos2d::backend::BufferType& type)
    {
        switch (type)
        {
            case cocos2d::backend::BufferType::VERTEX:
                return u8"VertexBuffer";
            case cocos2d::backend::BufferType::INDEX:
                return u8"IndexBuffer";
            default:
                return u8"";
        }
    }
}  // namespace

BufferNau::BufferNau(std::size_t size, cocos2d::backend::BufferType type, cocos2d::backend::BufferUsage usage) :
    Buffer(size, type, usage)
{
    _buffer = d3d::create_sbuffer(0, size, toNauUsage(usage) | toNauType(type), 0, toNauBufferNameType(type));
}

BufferNau::~BufferNau()
{
    if (_buffer)
    {
        _buffer->destroy();
    }
    _buffer = nullptr;
}

void BufferNau::usingDefaultStoredData(bool needDefaultStoredData)
{
}

void BufferNau::updateData(void* data, std::size_t size)
{
    NAU_ASSERT(size && size <= _size);

    _buffer->updateData(0, size, data, VBLOCK_WRITEONLY);
}

void BufferNau::updateSubData(void* data, std::size_t offset, std::size_t size)
{
    NAU_ASSERT(size && size <= _size);

    _buffer->updateData(offset, size, data, VBLOCK_WRITEONLY);
}

DAGOR_CC_BACKEND_END
