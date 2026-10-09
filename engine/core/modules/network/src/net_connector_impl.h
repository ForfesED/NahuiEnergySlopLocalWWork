// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// net_snapshots.h

#pragma once

#include <EASTL/map.h>
#include <EASTL/string.h>
#include <EASTL/utility.h>

#include "nau/network/napi/networking.h"
#include "nau/network/netsync/net_connector.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/rtti/rtti_object.h"
#include "nau/rtti/type_info.h"
#include "nau/service/service.h"

namespace nau
{
    class NetConnectorImpl final : public IServiceInitialization,
                                   public INetConnector
    {
        NAU_RTTI_CLASS(nau::NetConnectorImpl, IServiceInitialization, INetConnector)
    public:
        async::Task<> preInitService() override;
        async::Task<> initService() override;

        void init() override;
        void init(eastl::unique_ptr<INetworking>& net);

        void listen(const eastl::string& localPeerId, const eastl::string& remotePeerId, const eastl::string& url) override;
        void getListeners(eastl::vector<ConnectionData>& listeners) override;

        void connect(const eastl::string& localPeerId, const eastl::string& remotePeerId, const eastl::string& url) override;
        void getConnectors(eastl::vector<ConnectionData>& connectors) override;

        void getConnections(const eastl::string& peerId, eastl::vector<eastl::string>& peers) override;
        void getConnections(eastl::vector<eastl::weak_ptr<IConnection>>& connections) override;

        void writeFrame(const eastl::string& peerId, const eastl::string& frame) override;
        bool readFrame(const eastl::string& peerId, const eastl::string& fromPeerId, eastl::string& frame) override;

        void update() override;

    private:
        struct Connection : public IConnection
        {
            eastl::string m_localPeerId;
            eastl::string m_remotePeerId;
            State m_state = none;
            eastl::shared_ptr<INetworkingTransport> m_transport;

            // Temporary measure until we implement some message protocol for stream connections, like ASIO TCP
            eastl::string m_recBuffer;

            eastl::string m_frameBuffer;

            Connection(const eastl::string& localPeerId, const eastl::string& remotePeerId) :
                m_localPeerId(localPeerId),
                m_remotePeerId(remotePeerId)
            {
            }

            virtual State state() override
            {
                return m_state;
            }

            virtual const eastl::string& localPeerId() override
            {
                return m_localPeerId;
            }

            virtual const eastl::string& remotePeerId() override
            {
                return m_remotePeerId;
            }

            virtual const eastl::string& localEndPoint() override
            {
                return m_transport->localEndPoint();
            }

            virtual const eastl::string& remoteEndPoint() override
            {
                return m_transport->remoteEndPoint();
            }

            void update();
            void processMessages();
            void processMessage(const eastl::string& message);
            void writeFrame(const eastl::string& frame);
            void requestRemoteId();
            void sendId();

            // Debug
            bool m_verbose = true;
        };

        eastl::unique_ptr<INetworking> m_networking;
        eastl::vector<eastl::pair<eastl::shared_ptr<INetworkingListener>, ConnectionData>> m_listeners;
        eastl::vector<eastl::pair<eastl::shared_ptr<INetworkingConnector>, ConnectionData>> m_connectors;
        eastl::vector<eastl::shared_ptr<Connection>> m_connections;

        // Debug
        bool m_verbose = true;
    };
}  // namespace nau