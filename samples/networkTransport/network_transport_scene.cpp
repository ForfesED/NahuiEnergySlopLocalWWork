// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include <imgui.h>

#include "nau/app/run_application.h"
#include "nau/samples/components/camera_control.h"
#include "nau/samples/components/rotator.h"
#include "nau/samples/sample_app_delegate.h"

#include "nau/assets/asset_descriptor.h"
#include "nau/assets/asset_manager.h"
#include "nau/assets/asset_ref.h"
#include "nau/network/components/net_scene_component.h"
#include "nau/network/components/net_sync_component.h"
#include "nau/network/components/net_sync_transform_component.h"
#include "nau/network/netsync/net_connector.h"
#include "nau/scene/components/camera_component.h"
#include "nau/scene/components/static_mesh_component.h"
#include "nau/scene/scene_factory.h"

namespace nau::sample
{
    struct PlayerSetup
    {
        int idx;
        math::vec3 m_offset;
        unsigned posX;
        unsigned posY;

        eastl::string getName() const
        {
            return "Player" + eastl::to_string(idx);
        }
    };

    static PlayerSetup players[] = {
        {0,  {-4, 2, -6},   0,   0},
        {1,   {4, 2, -6}, 650,   0},
        {2, {-4, -2, -6},   0, 500},
        {3,  {4, -2, -6}, 650, 500}
    };

    async::Task<> makeScene(const PlayerSetup& player, bool isReplicated)
    {
        using namespace nau::scene;

        AssetRef<> sceneAssetRef{"file:/content/scenes/damaged_helmet/DamagedHelmet.gltf"};

        SceneAsset::Ptr sceneAsset = co_await sceneAssetRef.getAssetViewTyped<SceneAsset>();

        IScene::Ptr scene = getServiceProvider().get<scene::ISceneFactory>().createSceneFromAsset(*sceneAsset);

        SceneObject& sceneRoot = scene->getRoot();
        sceneRoot.setName("Root");
        sceneRoot.setTranslation(player.m_offset);

        auto& nsc = sceneRoot.addComponent<nau::NetSceneComponent>();
        nsc.setPeerId(player.getName().c_str());
        nsc.setSceneName(player.getName().c_str());

        auto childs = scene->getRoot().getChildObjects(true);

        for (auto& child : childs)
        {
            auto* staticMesh = child->findFirstComponent<StaticMeshComponent>();
            if (staticMesh != nullptr)
            {
                child->setName("Mesh");
                IComponentNetSync& nsct = child->addComponent<nau::NetSyncTransformComponent>();
                nsct.setIsReplicated(isReplicated);
                if (!isReplicated)
                {
                    auto& rotator1 = child->addComponent<MyRotator>();
                    rotator1.setRotationAxis(Axis::Y);
                    rotator1.setSpeedFactor(0.25f);
                }
                break;
            }
        }
        co_await getServiceProvider().get<scene::ISceneManager>().activateScene(std::move(scene));
        co_return;
    }

    void listenersTable()
    {
        eastl::vector<INetConnector::ConnectionData> listeners;
        getServiceProvider().get<INetConnector>().getListeners(listeners);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.0f, .0f, .0f, 0.05f));  // Set window background to red
        ImGui::Begin("Listeners");
        ImGui::SetWindowSize(ImVec2(300, 200), ImGuiCond_Once);
        ImGui::BeginTable("Listeners", 3, ImGuiTableFlags_SizingFixedFit);
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("Local");
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("Remote");
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("URL");
        ImGui::TableHeadersRow();
        for (auto& listener : listeners)
        {
            ImGui::TableNextColumn();
            ImGui::Text(listener.localPeerId.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(listener.remotePeerId.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(listener.localUri.c_str());
        }
        ImGui::EndTable();
        ImGui::End();
        ImGui::PopStyleColor();
    }

    void connectorsTable()
    {
        eastl::vector<INetConnector::ConnectionData> connectors;
        getServiceProvider().get<INetConnector>().getConnectors(connectors);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.0f, .0f, .0f, 0.05f));  // Set window background to red
        ImGui::Begin("Connectors");
        ImGui::SetWindowSize(ImVec2(300, 200), ImGuiCond_Once);
        ImGui::BeginTable("Connectors", 3, ImGuiTableFlags_SizingFixedFit);
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("Local");
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("Remote");
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("URL");
        ImGui::TableHeadersRow();
        for (auto& connector : connectors)
        {
            ImGui::TableNextColumn();
            ImGui::Text(connector.localPeerId.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(connector.remotePeerId.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(connector.localUri.c_str());
        }
        ImGui::EndTable();
        ImGui::End();
        ImGui::PopStyleColor();
    }

    void connectionsTable()
    {
        eastl::vector<eastl::weak_ptr<INetConnector::IConnection>> connections;
        getServiceProvider().get<INetConnector>().getConnections(connections);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.0f, .0f, .0f, 0.05f));  // Set window background to red
        ImGui::Begin("Connections");
        ImGui::SetWindowSize(ImVec2(600, 200), ImGuiCond_Once);
        ImGui::BeginTable("Connections", 4, ImGuiTableFlags_SizingFixedFit);
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("Local");
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("Remote");
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("EndPoint_Local");
        ImGui::TableNextColumn();
        ImGui::TableSetupColumn("EndPoint_Remote");
        ImGui::TableHeadersRow();
        for (auto& connector : connections)
        {
            auto cn = connector.lock();
            ImGui::TableNextColumn();
            ImGui::Text(cn->localPeerId().c_str());
            ImGui::TableNextColumn();
            ImGui::Text(cn->remotePeerId().c_str());
            ImGui::TableNextColumn();
            ImGui::Text(cn->localEndPoint().c_str());
            ImGui::TableNextColumn();
            ImGui::Text(cn->remoteEndPoint().c_str());
        }
        ImGui::EndTable();
        ImGui::End();
        ImGui::PopStyleColor();
    }

    const int MaxPlayers = 4;

    void startListeners(int playerIdx, const eastl::string& baseUrl, int basePort)
    {
        const eastl::string player("Player");
        eastl::string localPlayer(player + eastl::to_string(playerIdx));
        int localPortBase = basePort + playerIdx * 10;
        for (int i = 0; i < MaxPlayers; ++i)
        {
            if (i > playerIdx)
            {
                eastl::string remotePlayer(player + eastl::to_string(i));
                int localPort = localPortBase + i;
                eastl::string url = baseUrl + ":" + eastl::to_string(localPort) + "/";
                getServiceProvider().get<INetConnector>().listen(localPlayer, remotePlayer, url);
            }
        }
    }

    void startConnectors(int playerIdx, const eastl::string& baseUrl, int basePort)
    {
        const eastl::string player("Player");
        eastl::string localPlayer(player + eastl::to_string(playerIdx));
        int remotePortBase = basePort + playerIdx;
        for (int i = 0; i < MaxPlayers; ++i)
        {
            if (i < playerIdx)
            {
                eastl::string remotePlayer(player + eastl::to_string(i));
                int remotePort = remotePortBase + i * 10;
                eastl::string url = baseUrl + ":" + eastl::to_string(remotePort) + "/";
                getServiceProvider().get<INetConnector>().connect(localPlayer, remotePlayer, url);
            }
        }
    }

    async::Task<> startupDemo(sample::PlayerSetup* players, int playerIdx, eastl::string connectionAddress, eastl::string listenerAddress)
    {
        using namespace nau::async;
        using namespace nau::scene;

        auto& sceneManager = getServiceProvider().get<scene::ISceneManager>();
        auto& sceneFactory = getServiceProvider().get<scene::ISceneFactory>();

        {
            auto scene_00 = sceneFactory.createEmptyScene();
            scene_00->setName("scene_00");
            ObjectWeakRef cameraObject = scene_00->getRoot().attachChild(sceneFactory.createSceneObject<scene::CameraComponent>());
            cameraObject->setName("Camera.001");
            cameraObject->addComponent<CameraControl>();

            co_await sceneManager.activateScene(std::move(scene_00));
        }

        if (playerIdx == -1)
        {
            for (int i = 0; i < 2; ++i)
            {
                startListeners(i, "tcp://127.0.0.1", 9900);
                startConnectors(i, "tcp://127.0.0.1", 9900);
                co_await sample::makeScene(players[i], false);
            }
        }
        else
        {
            // Default listenerAddress connectionAddress and is 127.0.0.1
            startListeners(playerIdx, "tcp://" + listenerAddress, 9900);
            startConnectors(playerIdx, "tcp://" + connectionAddress, 9900);
            co_await sample::makeScene(players[playerIdx], false);
        }
    }

    void setOnSceneMissing(eastl::string_view peerId, eastl::string_view sceneName)
    {
        using namespace nau::async;
        using namespace nau::scene;

        static eastl::set<eastl::string> peers;
        if (peers.count(peerId.data()) == 0)
        {
            peers.emplace(peerId);
            for (auto& player : players)
            {
                if (player.getName() == peerId)
                {
                    sample::makeScene(player, false).detach();
                    return;
                }
            }
        }
    }

    /**
     */
    class MySampleDelegate final : public SampleAppDelegate
    {
    public:
        MySampleDelegate(int argc, char** argv) :
            SampleAppDelegate("networkTransport"),
            m_argc(argc),
            m_argv(argv)
        {
        }

    private:

        Result<> initializeServices() override
        {
            getServiceProvider().addClass<nau::sample::CameraControl>();

            getServiceProvider().addClass<nau::NetSceneComponent>();
            getServiceProvider().addClass<nau::NetSyncTransformComponent>();
            getServiceProvider().addClass<nau::sample::MyRotator>();
            return ResultSuccess;
        }


        async::Task<> startupApplication() override
        {
            int peerId = -1;
            if (m_argc > 1)
            {
                peerId = std::atoi(m_argv[1]);
            }
            eastl::string connectionAddress = "127.0.0.1";
            if (m_argc > 2)
            {
                // Setup IP address for startConnectors()
                connectionAddress = m_argv[2];
            }
            eastl::string listenerAddress = "127.0.0.1";
            if (m_argc > 3)
            {
                // Setup IP address for startListeners()
                listenerAddress = m_argv[3];
            }

            if (auto* windowService = getServiceProvider().find<IWindowManager>())
            {
                auto& window = windowService->getActiveWindow();
                for (auto& player : sample::players)
                {
                    if (player.idx == peerId)
                    {
                        window.setPosition(player.posX, player.posY);
                        window.setName(player.getName().c_str());
                    }
                }
            }

            getServiceProvider().get<INetConnector>().init();

            if (peerId == -1)
            {
                auto res = getServiceProvider().get<INetSnapshots>().doSelfTest();
                if (res)
                {
                    NAU_LOG_DEBUG("NetSnapshots self test passed");
                }
                else
                {
                    NAU_FAILURE("NetSnapshots self test failed");
                }
            }

            getServiceProvider().get<INetSnapshots>().setOnSceneMissing(nau::sample::setOnSceneMissing);

            co_await sample::startupDemo(sample::players, peerId, connectionAddress, listenerAddress);
        }

        void onApplicationStep([[maybe_unused]] std::chrono::milliseconds dt) override
        {
            auto& sceneManager = getServiceProvider().get<scene::ISceneManager>();
            auto scenes = sceneManager.getActiveScenes();

            // ImGui::Text("Component name: ");

            nau::sample::listenersTable();
            nau::sample::connectorsTable();
            nau::sample::connectionsTable();
        }

        const int m_argc;
        char** const m_argv;
    };
}  // namespace nau::sample

namespace nau
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(NetSceneComponent);
    NAU_IMPLEMENT_DYNAMIC_OBJECT(NetSyncBaseComponent);
    NAU_IMPLEMENT_DYNAMIC_OBJECT(NetSyncTransformComponent);
}  // namespace nau

int main_scene(int argc, char** argv)
{
    using namespace nau;
    return runApplication(eastl::make_unique<sample::MySampleDelegate>(argc, argv));
}
