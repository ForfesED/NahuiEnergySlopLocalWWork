/****************************************************************************
 Copyright (c) 2015-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
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

#include "base/ccTypes.h"
#include "base/CCRef.h"
#include "base/CCEventListenerCustom.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCGroupCommand.h"
#include "renderer/backend/Types.h"
#include <vector>

NS_CC_BEGIN

class CameraBackgroundColorBrush;
class CameraBackgroundDepthBrush;
class Camera;

namespace backend {
    class ProgramState;
    class Buffer;
}

/**
 * Defines a brush to clear the background of camera.
 * There are 4 types of brush. None brush do nothing, Depth brush clear background with given depth, Color brush clear background with given color and depth, Skybox brush clear the background with a skybox. Camera uses depth brush by default.
 */
class CC_DLL CameraBackgroundBrush : public Ref
{
public:
    /**
     * Brush types. There are 4 types of brush. See CameraBackgroundDepthBrush, CameraBackgroundColorBrush for more information.
     */
    enum class BrushType
    {
        NONE, //none brush
        DEPTH, // depth brush. See CameraBackgroundDepthBrush
        COLOR, // color brush. See CameraBackgroundColorBrush
    };
    
    /**
     * get brush type
     * @return BrushType
     */
    virtual BrushType getBrushType() const { return BrushType::NONE; }
    
    /**
     * Creates a none brush, it does nothing when clear the background
     * @return Created brush.
     */
    static CameraBackgroundBrush* createNoneBrush();
    
    /**
     * Creates a depth brush, which clears depth buffer with a given depth.
     * @param depth Depth used to clear depth buffer
     * @return Created brush
     */
    static CameraBackgroundDepthBrush* createDepthBrush(float depth = 1.f);
    
    /**
     * Creates a color brush
     * @param color Color of brush
     * @param depth Depth used to clear depth buffer
     * @return Created brush
     */
    static CameraBackgroundColorBrush* createColorBrush(const Color4F& color, float depth);
    
    /**
     * draw the background
     */
    virtual void drawBackground(Camera* /*camera*/) {}

    virtual bool isValid() { return true; }

CC_CONSTRUCTOR_ACCESS :
    CameraBackgroundBrush();
    virtual ~CameraBackgroundBrush();

    virtual bool init() { return true; }
    
protected:
    backend::ProgramState* _programState = nullptr;
};

/**
 * Depth brush clear depth buffer with given depth
 */
class CC_DLL CameraBackgroundDepthBrush : public CameraBackgroundBrush
{
public:
    /**
     * Create a depth brush
     * @param depth Depth used to clear the depth buffer
     * @return Created brush
     */
    static CameraBackgroundDepthBrush* create(float depth);
    
    /**
     * Get brush type. Should be BrushType::DEPTH
     * @return brush type
     */
    virtual BrushType getBrushType() const override { return BrushType::DEPTH; }
    
    /**
     * Draw background
     */
    virtual void drawBackground(Camera* camera) override;
    
    /**
     * Set depth
     * @param depth Depth used to clear depth buffer
     */
    void setDepth(float depth) { _depth = depth; }
    
CC_CONSTRUCTOR_ACCESS:
    CameraBackgroundDepthBrush();
    virtual ~CameraBackgroundDepthBrush();

    virtual bool init() override;
private:
    void onBeforeDraw();
    void onAfterDraw();
protected:
#if CC_ENABLE_CACHE_TEXTURE_DATA
    EventListenerCustom* _backToForegroundListener;
#endif
    void initBuffer();

protected:
    float _depth;
    backend::UniformLocation _locDepth;
    CustomCommand _customCommand;
    GroupCommand _groupCommand;

    bool _clearColor;
    std::vector<V3F_C4B_T2F> _vertices;
    struct {
        uint32_t stencilWriteMask = 0;
        bool depthTest = true;
        backend::CompareFunction compareFunc = backend::CompareFunction::ALWAYS;
    } _stateBlock;

};

/**
 * Color brush clear buffer with given depth and color
 */
class CC_DLL CameraBackgroundColorBrush : public CameraBackgroundDepthBrush
{
public:
    /**
     * Get brush type. Should be BrushType::COLOR
     * @return brush type
     */
    virtual BrushType getBrushType() const override { return BrushType::COLOR; }
    
    /**
     * Create a color brush
     * @param color Color used to clear the color buffer
     * @param depth Depth used to clear the depth buffer
     * @return Created brush
     */
    static CameraBackgroundColorBrush* create(const Color4F& color, float depth);
    
    /**
     * Draw background
     */
    virtual void drawBackground(Camera* camera) override;
    
    /**
     * Set clear color
     * @param color Color used to clear the color buffer
     */
    void setColor(const Color4F& color);

CC_CONSTRUCTOR_ACCESS:
    CameraBackgroundColorBrush();
    virtual ~CameraBackgroundColorBrush();

    virtual bool init() override;
    
protected:
    Color4F _color;
};

class TextureCube;
class GLProgramState;
class EventListenerCustom;

NS_CC_END

