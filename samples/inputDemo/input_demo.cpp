// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <imgui.h>

#include "input_control_component.h"
#include "input_rotate_component.h"
#include "input_test.h"
#include "nau/app/run_application.h"
#include "nau/assets/asset_container.h"
#include "nau/assets/asset_container_builder.h"
#include "nau/assets/asset_manager.h"
#include "nau/assets/asset_ref.h"
#include "nau/dag_ioSys/dag_chainedMemIo.h"
#include "nau/input.h"
#include "nau/input_system.h"
#include "nau/samples/sample_app_delegate.h"
#include "nau/samples/components/camera_control.h"
#include "nau/scene/camera/camera_manager.h"
#include "nau/scene/components/camera_component.h"
#include "nau/scene/components/component_life_cycle.h"
#include "nau/scene/components/static_mesh_component.h"
#include "nau/scene/scene.h"
#include "nau/scene/scene_factory.h"
#include "nau/scene/scene_manager.h"
#include "nau/app/core_window_manager.h"
#include "nau/platform/windows/app/windows_window.h"
#include "nau/graphics/core_graphics.h"
#include "nau/render/render_window.h"

namespace nau::sample
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(InputControlComponent)
    NAU_IMPLEMENT_DYNAMIC_OBJECT(InputRotateComponent)

    async::Task<scene::IScene::Ptr> loadGltfScene(eastl::string sceneAssetPath, bool setupCamera)
    {
        using namespace nau::scene;
        AssetRef<> sceneAssetRef{AssetPath{sceneAssetPath}};
        SceneAsset::Ptr sceneAsset = co_await sceneAssetRef.getAssetViewTyped<SceneAsset>();
        IScene::Ptr scene = getServiceProvider().get<scene::ISceneFactory>().createSceneFromAsset(*sceneAsset);
        co_return scene;
    }
}  // namespace nau::sample

void replaceStr(eastl::string& str, const eastl::string& src, const eastl::string& dst)
{
    auto index = str.find(src, 0);
    if (index == eastl::string::npos)
    {
        return;
    }
    str.replace(index, src.size(), dst);
}

void updateGUI()
{
    using namespace nau;
    auto& insys = getServiceProvider().get<IInputSystem>();
    auto devices = insys.getDevices();
    int posx = 20;
    int posy = -180;
    int sizex = 500;
    int sizey = 250;
    int rowSize = 16;
    for (auto& device : devices)
    {
        // std::cout << "Device " << device->getName().c_str() << std::endl;
        ImGui::SetNextWindowBgAlpha(0.15f);
        ImGui::Begin(device->getName().c_str());
        ImGui::SetWindowPos(ImVec2(posx, posy += sizey), ImGuiCond_Once);
        ImGui::SetWindowSize(ImVec2(sizex, sizey), ImGuiCond_Once);
        {
            unsigned num = device->getKeysNum();
            for (unsigned i = 0; i < num; ++i)
            {
                auto state = device->getKeyState(i);
                auto name = device->getKeyName(i);
                while (name.size() < 3)
                {
                    name = " " + name + " ";
                }
                replaceStr(name, "mouse", "m");
                if (state == IInputDevice::Pressed)
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), name.substr(0, 3).c_str());
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 1, 1, 1), name.substr(0, 3).c_str());
                }
                if ((i % rowSize) != (rowSize - 1))
                {
                    ImGui::SameLine();
                }
            }
        }
        ImGui::End();
    }
}

namespace nau::sample
{
    void getSources(eastl::vector<eastl::shared_ptr<InputSource>>& sources)
    {
        sources.clear();
        auto* const coreGraphics = getServiceProvider().find<nau::ICoreGraphics>();
        if (coreGraphics != nullptr)
        {
            eastl::vector<WeakPtr<nau::render::IRenderWindow>> windows;
            coreGraphics->getRenderWindows(windows);
            for (auto& windowPtr : windows)
            {
                auto ptr = windowPtr.lock();
                if (ptr)
                {
                    sources.push_back(eastl::make_shared<InputSource>(InputSource({
                        (uintptr_t)ptr->getHwnd(),
                        {ptr->getName().data(), ptr->getName().size()}
                    })));
                }
            }
        }
    }

    class MySampleDelegate final : public SampleAppDelegate
    {
    public:
        MySampleDelegate() :
            SampleAppDelegate("inputDemo")
        {
        }

    private:

        Result<> initializeServices() override
        {
            getServiceProvider().addClass<nau::sample::InputControlComponent>();
            getServiceProvider().addClass<nau::sample::InputRotateComponent>();
            getServiceProvider().addClass<nau::sample::CameraControl>();
            return ResultSuccess;
        }

        async::Task<> startupApplication() override
        {
            scene::IScene::Ptr mainScene = co_await loadGltfScene("file:/content/scenes/damaged_helmet/DamagedHelmet.gltf", false);
            onSceneLoaded(mainScene->getRoot());

            auto& inputSourceManager = getServiceProvider().get<IInputSourceManager>();
            inputSourceManager.setGetSources(getSources);
            auto& sceneManager = getServiceProvider().get<scene::ISceneManager>();
            auto& sceneFactory = getServiceProvider().get<scene::ISceneFactory>();
            auto* const windowManager = getServiceProvider().find<nau::ICoreWindowManager>();

            auto scene_00 = sceneFactory.createEmptyScene();
            scene_00->setName("scene_00");

            nau::scene::ObjectWeakRef<nau::scene::SceneObject> cameraObject;
            cameraObject = scene_00->getRoot().attachChild(sceneFactory.createSceneObject());
            cameraObject->setName("Camera.Main");
            m_camera = getServiceProvider().get<nau::scene::ICameraManager>().createDetachedCamera();
            m_camera->setCameraName("Camera.Main");

            co_await sceneManager.activateScene(std::move(scene_00));
            co_await sceneManager.activateScene(std::move(mainScene));

            if (windowManager)
            {
                auto window = windowManager->createWindow(false);
                window->setVisible(true);
                auto pos = window->getPosition();
                window->setPosition(pos.first + 200, pos.second);
                auto* const coreGraphics = getServiceProvider().find<nau::ICoreGraphics>();
                m_task = coreGraphics->createRenderWindow(window->as<nau::IWindowsWindow*>()->getWindowHandle());
                m_windows.emplace_back(std::move(window));
            }
            else
            {
                NAU_LOG_WARNING("No ICoreWindowManager service found");
            }

            // TODO: Commented because
            // MemorySaveCB::write is broken:
            // - MemoryChainedData::create(msz, m); ->
            // - mcd->size = getDefaultAllocator()->getSize(mcd) - sizeof(MemoryChainedData) + 16;
            //  getDefaultAllocator()->getSize(mcd) - returns invalid value
#if 0
            // nau::input::dumpDevices();
            nau::input::testFullCycle(true);
            nau::input::testContexts();
            // nau::input::dumpToGraph();
#endif
            co_return;
        }

        void onSceneLoaded(nau::scene::SceneObject& root)
        {
            for (nau::scene::SceneObject* const obj : root.getChildObjects(true))
            {
                if (auto* const staticMesh = obj->findFirstComponent<nau::scene::StaticMeshComponent>(); staticMesh)
                {
                    auto& control = obj->addComponent<InputControlComponent>();
                    control.setInputContexts("MainRenderView");
                    auto& rotation = obj->addComponent<InputRotateComponent>();
                    rotation.setInputContexts("RenderWindow #0");
                }
            }
        }

        void onApplicationStep([[maybe_unused]] std::chrono::milliseconds dt) override
        {
            float delta = nau::input::getMouseAxisDelta(0, nau::input::MouseKey::Wheel);
            if (delta != 0.f)
            {
                NAU_LOG_INFO("Scroll {}", delta);
            }
            updateGUI();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        async::Task<nau::render::IRenderWindow::WeakRef> m_task;
        eastl::list<nau::Ptr<IPlatformWindow>> m_windows;

        nau::Ptr<nau::scene::ICameraControl> m_camera;
    };
}  // namespace nau::sample

int main(int argc, char** argv)
{
    using namespace nau;
    return runApplication(eastl::make_unique<sample::MySampleDelegate>());
}
