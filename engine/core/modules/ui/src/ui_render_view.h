// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <cocos/platform/CCRenderView.h>


namespace nau::ui
{


class UIRenderView final : cocos2d::RenderView
{
public:
    UIRenderView() = default;
    virtual ~UIRenderView();
    static cocos2d::RenderView* create(float width, float height);

    virtual void end() override;
    virtual bool isOpenGLReady() override {return true;};
    virtual void swapBuffers() override {};
    virtual void setIMEKeyboardState(bool open) override {};

    virtual void setApplicationDidEnterBackgroundCb(void (*callback)()) override {};
    virtual void setApplicationWillEnterForegroundCb(void (*callback)()) override {};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    virtual HWND getWin32Window() override {return nullptr; }
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    virtual id getCocoaWindow() override { return 0; };
    virtual id getNSGLContext() override {retrurn 0; }; // stevetranby: added
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) */
};


}
