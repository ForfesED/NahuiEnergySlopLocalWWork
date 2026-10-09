// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// net_snapshots_impl.cpp

#include "net_connector_impl.h"

#include "nau/network/napi/networking_factory.h"
#include "nau/service/service_provider.h"

namespace nau
{
    async::Task<> NetConnectorImpl::preInitService()
    {
        return async::Task<>::makeResolved();
    }

    async::Task<> NetConnectorImpl::initService()
    {
        return async::Task<>::makeResolved();
    }

    void NetConnectorImpl::init()
    {
        if (!m_networking)
        {
            auto service = nau::getServiceProvider().find<NetworkingFactory>();
            m_networking = service->create("ASIO");
            m_networking->init();
        }
    }

    void NetConnectorImpl::init(eastl::unique_ptr<INetworking>& net)
    {
        m_networking = eastl::move(net);
    }

    void NetConnectorImpl::listen(const eastl::string& localPeerId, const eastl::string& remotePeerId, const eastl::string& url)
    {
        auto listener = m_networking->createListener();
        m_listeners.push_back({
            listener,
            {localPeerId, remotePeerId, url}
        });
        listener->listen(
            url,
            [this, localPeerId, remotePeerId](eastl::shared_ptr<nau::INetworkingTransport> incomingTransport) -> void
        {
            auto connection = eastl::make_shared<Connection>(localPeerId, remotePeerId);
            connection->m_transport = incomingTransport;
            connection->m_state = Connection::State::accepted;
            m_connections.push_back(connection);
        },
            []() -> void
        {
        });
    }

    void NetConnectorImpl::getListeners(eastl::vector<ConnectionData>& listeners)
    {
        listeners.clear();
        listeners.reserve(m_listeners.size());
        for (auto& listener : m_listeners)
        {
            listeners.push_back(listener.second);
        }
    }

    void NetConnectorImpl::connect(const eastl::string& localPeerId, const eastl::string& remotePeerId, const eastl::string& url)
    {
        auto connector = m_networking->createConnector();
        m_connectors.push_back({
            connector,
            {localPeerId, remotePeerId, url}
        });
        connector->connect(
            url,
            [this, localPeerId, remotePeerId, url](eastl::shared_ptr<nau::INetworkingTransport> outgoingTransport) -> void
        {
            auto connection = eastl::make_shared<Connection>(localPeerId, url);
            connection->m_remotePeerId = remotePeerId;
            connection->m_transport = outgoingTransport;
            connection->m_state = Connection::State::connected;
            m_connections.push_back(connection);
        },
            []() -> void
        {
        });
    }

    void NetConnectorImpl::getConnectors(eastl::vector<ConnectionData>& connectors)
    {
        connectors.clear();
        connectors.reserve(m_connectors.size());
        for (auto& connector : m_connectors)
        {
            connectors.push_back(connector.second);
        }
    }

    void NetConnectorImpl::getConnections(const eastl::string& peerId, eastl::vector<eastl::string>& peers)
    {
        peers.clear();
        for (auto& connection : m_connections)
        {
            if (connection->m_localPeerId == peerId)
            {
                if (!connection->m_remotePeerId.empty())
                {
                    peers.push_back(connection->m_remotePeerId);
                }
            }
        }
    }

    void NetConnectorImpl::getConnections(eastl::vector<eastl::weak_ptr<IConnection>>& connections)
    {
        connections.clear();
        for (auto& connection : m_connections)
        {
            connections.push_back(connection);
        }
    }

    void NetConnectorImpl::writeFrame(const eastl::string& peerId, const eastl::string& frame)
    {
        for (auto& connection : m_connections)
        {
            if (connection->m_localPeerId == peerId)
            {
                connection->writeFrame(frame);
            }
        }
    }

    bool NetConnectorImpl::readFrame(const eastl::string& peerId, const eastl::string& fromPeerId, eastl::string& frame)
    {
        frame.clear();
        for (auto& connection : m_connections)
        {
            if (connection->m_localPeerId == peerId && connection->m_remotePeerId == fromPeerId)
            {
                if (!connection->m_frameBuffer.empty())
                {
                    frame = connection->m_frameBuffer;
                    return true;
                }
            }
        }
        return false;
    }

    void NetConnectorImpl::update()
    {
        if (m_networking)
        {
            m_networking->update();
        }
        for (auto& connection : m_connections)
        {
            connection->update();
        }
    }

    void NetConnectorImpl::Connection::update()
    {
        if (m_transport)
        {
            if (m_transport->isConnected())
            {
                eastl::vector<NetworkingMessage> messages;
                m_transport->read(messages);
                // TODO - move message handling to ASIO transport implementation
                for (auto& message : messages)
                {
                    m_recBuffer.append(asStringView(message.buffer).data());
                }
                processMessages();
                if (m_remotePeerId.empty())
                {
                    requestRemoteId();
                }
            }
        }
    }

    void NetConnectorImpl::Connection::processMessages()
    {
        while (true)
        {
            size_t start = m_recBuffer.find('\n');
            if (start == eastl::string::npos)
            {
                return;
            }
            size_t end = m_recBuffer.find('\r', start);
            if (end == eastl::string::npos)
            {
                return;
            }
            eastl::string message = m_recBuffer.substr(start, end - start + 1);
            m_recBuffer.erase(start, end - start + 1);
            processMessage(message);
        }
    }

    void NetConnectorImpl::Connection::processMessage(const eastl::string& message)
    {
        if (message == "\n|req_id\r")
        {
            sendId();
            return;
        }
        if (message.find("\n|id", 0) == 0)
        {
            m_remotePeerId = message.substr(4, message.size() - 5);
            return;
        }
        if (message.find("{") == 1)  // frame data assumed
        {
            m_frameBuffer = message;
        }
    }

    void NetConnectorImpl::Connection::writeFrame(const eastl::string& frame)
    {
        NetworkingMessage message("\n" + frame + "\r");
        m_transport->write(message);
    }

    void NetConnectorImpl::Connection::requestRemoteId()
    {
        NetworkingMessage message("\n|req_id\r");
        m_transport->write(message);
        if (m_verbose)
        {
            // NAU_LOG_DEBUG("requestRemoteId()");
        }
    }

    void NetConnectorImpl::Connection::sendId()
    {
        NetworkingMessage message("\n|id" + m_localPeerId + "\r");
        m_transport->write(message);
        if (m_verbose)
        {
            // NAU_LOG_DEBUG("sendId()");
        }
    }

}  // namespace nau