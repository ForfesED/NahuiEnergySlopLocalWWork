// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <Windows.h>
#include <cocos/base/CCEventKeyboard.h>
#include <cocos/base/CCEventMouse.h>
#include <profileapi.h>
#include <windef.h>

#include <cstdint>
#include <thread>

#include "2d/CCScene.h"
#include "EASTL/unordered_map.h"
#include "EASTL/vector.h"
#include "nau/input.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/rtti/rtti_object.h"
#include "nau/service/service.h"
#include "nau/ui.h"
#include "nau/utils/uid.h"
#include "nau/3d/dag_tex3d.h"

namespace nau::ui
{
    class UiManagerImpl final : public IServiceInitialization,
                                public IServiceShutdown,
                                public UiManager
    {
        NAU_RTTI_CLASS(
            nau::ui::UiManagerImpl,
            nau::IServiceInitialization,
            nau::IServiceShutdown,
            nau::ui::UiManager)

        struct ElementChangedCallbackEntry 
        {
            nau::Uid id;
            TElementChangedCallback callback;
        };

    public:
        virtual async::Task<> preInitService() override;
        virtual async::Task<> initService() override;
        virtual async::Task<> shutdownService() override;

        virtual void applicationDidEnterBackground() override;
        virtual void applicationWillEnterForeground() override;
        virtual void setAnimationInterval(float interval) override;
        virtual void update() override;
        virtual void update(float dt) override;
        virtual void render(BaseTexture* backBuffer) override;
        virtual bool shouldShutDown() const override;
        virtual void setScreenSize(float width, float height) override;
        virtual void setReferenceResolution(float width, float height) override;

        virtual void addCanvas(Canvas* canvas) override;
        virtual Canvas* getCanvas(const eastl::string& canvasName) override;
        virtual void removeCanvas(const eastl::string& canvasName) override;
        virtual void updateCanvases() override;
        virtual TCallbackHandle setOnCanvasLoadedCallback(TCanvasLoadedCallback&& callback) override;
        virtual bool setOnCanvasLoadedCallback(TCallbackHandle handle) override;

        virtual void configureResourcePath() override;

        virtual eastl::vector<const rtti::TypeInfo*> getServiceDependencies() const override;

        virtual void setEngineScene(scene::IScene::WeakRef scene) override;
        virtual scene::IScene* getEngineScene() override;

        virtual Node* getCanvasChildUnderCursor(const eastl::string& canvasName) override;
        virtual void setElementChangedCallback(nau::Uid elementUid, TElementChangedCallback&& callback) override;
        virtual void removeElementChangedCallback(nau::Uid id) override;

    private:
        void handleInput();
        void touchBegin(int num, intptr_t ids[], const float xs[], const float ys[]);
        void touchMove(const float xs[], const float ys[]);
        void touchesMove(int num, intptr_t ids[], const float xs[], const float ys[], float fs[], float ms[]);
        void touchEnd(int num, intptr_t ids[], const float xs[], const float ys[]);
        bool applicationDidFinishLaunching();
        void initGLContextAttrs();
        void notifyElementsChanged();

        void invokeElementChangedCallbacks(class Node* node);
        void cleanupCallbacks();

        eastl::pair<float, float> getInputPosition(float axisX, float axisY) const;

    private:
        // HINSTANCE m_instance;
        HACCEL m_accelTable;
        LARGE_INTEGER m_animationInterval;
        eastl::string m_resourceRootPath;
        eastl::string m_startupScriptFilename;

        UINT wTimerRes = 0;
        LARGE_INTEGER m_nLast;
        LARGE_INTEGER m_nNow;
        LARGE_INTEGER m_freq;

        LONGLONG m_interval = 0LL;

        bool m_isAlive = false;

        float m_screenHeight = 0;
        float m_screenWidght = 0;

        float m_screenHeightReference = -1.0f;
        float m_screenWidghtReference = -1.0f;

        eastl::unordered_map<nau::input::MouseKey, cocos2d::EventMouse::MouseButton> m_mouseButtonRemap;
        bool m_touchCaptured = false;

        std::mutex m_cocosRenderMutex;

        Canvas* m_currentScene = nullptr;
        cocos2d::Scene* m_cocoScene = nullptr;

        eastl::unordered_map<eastl::string, Canvas*> m_canvases;

        eastl::unordered_map<TCallbackHandle, TCanvasLoadedCallback> m_canvasLoadedCbs;
        eastl::vector<TCallbackHandle> freeCanvasLoadedCallbackHandles;
        TCallbackHandle highestFreeCanvasLoadedCallbackHandle = 1;
        std::mutex m_canvasLoadedCbMutex {};

#if NAU_UI_CALLBACK_ON_ELEMNT_CHANGE
        eastl::vector<ElementChangedCallbackEntry> m_callbacks;
        std::mutex m_elementChangedCbMutex {};
        bool m_needsCleanup {false};
#endif

        scene::IScene::WeakRef m_engineScene;
    };
}  // namespace nau::ui
