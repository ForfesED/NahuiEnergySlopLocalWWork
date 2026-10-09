// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./scene_reloader.h"
#include "nau/input.h"
#include "nau/scene/scene_manager.h"


namespace nau::sample
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(SceneReloader)

    void SceneReloader::updateComponent(float dt)
    {
        using namespace nau::scene;

        ImGui::Begin("Demo");
        ImGui::SetWindowSize(ImVec2(150, 160), ImGuiCond_Once);

        ImGui::Text("Active scene:");
        ImGui::Text("Press to unload:");
        auto& sceneManager = getServiceProvider().get<scene::ISceneManager>();

        for (ObjectWeakRef sceneRef :sceneManager .getActiveScenes())
        {
            auto label = ::fmt::format("scene:({})", sceneRef->getName());
            if (ImGui::Button(label.c_str()))
            {
                unloadScene(sceneRef);
                break;
            }
        }

        ImGui::End();
    }

    void SceneReloader::unloadScene(scene::IScene::WeakRef scene)
    {
        NAU_LOG("Start unload scene");

        auto& sceneManager = getServiceProvider().get<scene::ISceneManager>();
        sceneManager.deactivateScene(scene);

        NAU_LOG("End unload scene");
    }
}  // namespace nau::sample
