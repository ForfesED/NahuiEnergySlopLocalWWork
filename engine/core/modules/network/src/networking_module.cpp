// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/app/main_loop/game_system.h"
#include "nau/dispatch/class_descriptor.h"
#include "nau/module/module.h"
#include "nau/network/asio/networking_asio.h"
#include "nau/network/transportTest/networking_test.h"
#include "nau/rtti/rtti_impl.h"
#include "net_connector_impl.h"
#include "net_snapshots_impl.h"
#include "networking_factory_impl.h"

namespace nau
{
    class NetworkGameSystem final : public IGamePreUpdate,
                                    public IGamePostUpdate,
                                    public IRttiObject
    {
        NAU_RTTI_CLASS(nau::NetworkGameSystem, IGamePostUpdate, IGamePreUpdate, IRttiObject)

    private:
        void gamePreUpdate([[maybe_unused]] std::chrono::milliseconds dt) override
        {
            getServiceProvider().get<INetConnector>().update();
            getServiceProvider().get<INetSnapshots>().applyPeerUpdates();
        }

        void gamePostUpdate([[maybe_unused]] std::chrono::milliseconds dt) override
        {
            getServiceProvider().get<INetSnapshots>().nextFrame();
        }
    };

    /**
     */
    class NetModule : public DefaultModuleImpl
    {
        string getModuleName() override
        {
            return "nau.net";
        }

        void initialize() override
        {
            NetworkingFactoryImpl::Register("Test", NetworkingTest::create);
            NetworkingFactoryImpl::Register("ASIO", NetworkingASIO::create);

            NAU_MODULE_EXPORT_SERVICE(nau::NetworkingFactoryImpl);
            NAU_MODULE_EXPORT_SERVICE(nau::NetSnapshotsImpl);
            NAU_MODULE_EXPORT_SERVICE(nau::NetConnectorImpl);
            NAU_MODULE_EXPORT_CLASS(nau::NetworkGameSystem);
        }
    };
}  // namespace nau

IMPLEMENT_MODULE(nau::NetModule)
