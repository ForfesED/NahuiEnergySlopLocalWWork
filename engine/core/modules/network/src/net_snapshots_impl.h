// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// net_snapshots.h

#pragma once

#include <EASTL/map.h>
#include <EASTL/string.h>

#include "nau/network/netsync/net_snapshots.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/rtti/rtti_object.h"
#include "nau/rtti/type_info.h"
#include "nau/service/service.h"

namespace nau
{
    class NetSnapshotsImpl final : public IServiceInitialization,
                                   public INetSnapshots
    {
        NAU_RTTI_CLASS(nau::NetSnapshotsImpl, INetSnapshots)
    public:
        async::Task<> preInitService() override;
        async::Task<> initService() override;

        void onSceneActivated(IComponentNetScene* scene) override;
        void onSceneDectivated(IComponentNetScene* scene) override;
        void onSceneUpdated(IComponentNetScene* scene) override;
        void setOnSceneMissing(nau::Functor<void(eastl::string_view peerId, eastl::string_view sceneName)> callback) override;

        void onComponentActivated(IComponentNetSync* component) override;
        void onComponentDeactivated(IComponentNetSync* component) override;
        void onComponentWrite(IComponentNetSync* component) override;

        void nextFrame() override;
        void applyPeerUpdates();
        void applyFrameUpdate(const eastl::string& peerId, uint32_t frame);

        // Debug
        void applyPeerUpdatesLocal(const char* srcPeerId, const char* dstPeerId);

        bool doSelfTest() override;

    private:
        // Seriazable
        struct ComponentData
        {
            NAU_CLASS_FIELDS(
                CLASS_FIELD(m_data))

            ComponentData() = default;
            ComponentData(IComponentNetSync* component);
            eastl::string m_data;
        };

        // Seriazable
        struct SceneSnapshot
        {
            NAU_CLASS_FIELDS(
                CLASS_FIELD(m_components))

            eastl::map<eastl::string, ComponentData> m_components;
            void writeComponent(IComponentNetSync* component);
        };

        // Seriazable
        struct FrameSnapshot
        {
            NAU_CLASS_FIELDS(
                CLASS_FIELD(m_frame),
                CLASS_FIELD(m_scenes))

            FrameSnapshot() = default;

            FrameSnapshot(uint32_t frame) :
                m_frame(frame)
            {
            }
            uint32_t m_frame = 0;
            eastl::map<eastl::string, SceneSnapshot> m_scenes;
            void writeComponent(const eastl::string& sceneName, IComponentNetSync* component);
        };

        // Local, not serializable
        struct PeerData
        {
            uint32_t m_currentFrame;
            eastl::map<eastl::string, IComponentNetScene*> m_peerScenes;
            eastl::map<uint32_t, FrameSnapshot> m_frames;

            void activateScene(IComponentNetScene* scene);
            void deactivateScene(IComponentNetScene* scene);

            void writeComponent(const eastl::string& sceneName, IComponentNetSync* component);

            void advanceToFrame(uint32_t frame);
            void purgeFrames(uint32_t frame);

            void serializeFrame(uint32_t frame, eastl::string& str);
            void deserializeFrame(const eastl::string& str);
        };

        PeerData* getPeer(const eastl::string& sceneName);

        uint32_t m_frame = 0;
        eastl::map<eastl::string, PeerData> m_peers;
        eastl::map<eastl::string, PeerData*> m_sceneToPeer;
        nau::Functor<void(eastl::string_view peerId, eastl::string_view sceneName)> m_onSceneMissing;
    };
}  // namespace nau