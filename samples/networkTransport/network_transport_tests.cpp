// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/diag/logging.h"
#include "nau/network/napi/networking_factory.h"

#define WINDOW_SERVICE

void ProcessTransport(nau::INetworkingTransport* transport, const eastl::string& prefix)
{
    nau::NetworkingMessage outMessage("Hello from " + prefix);
    transport->write(outMessage);

    eastl::vector<nau::NetworkingMessage> inMessages;
    transport->read(inMessages);
    for (auto& message : inMessages)
    {
        NAU_LOG_DEBUG(nau::string::format("{} got {} {}", prefix.c_str(), (unsigned int)message.buffer.size(), (const char*)message.buffer.data()).c_str());
    }
}

void DoTestSinglePeer(nau::INetworking* network)
{
    // Single connection
    auto connection_serv = network->createListener();
    auto connection_client = network->createConnector();
    eastl::shared_ptr<nau::INetworkingTransport> incoming;
    eastl::shared_ptr<nau::INetworkingTransport> outgoing;

    connection_serv->listen(
        "tcp://127.0.0.1:9999/",
        [&incoming](eastl::shared_ptr<nau::INetworkingTransport> incomingTransport) -> void
    {
        incoming = incomingTransport;
    },
        []() -> void
    {
    });
    connection_client->connect(
        "tcp://127.0.0.1:9999/",
        [&outgoing](eastl::shared_ptr<nau::INetworkingTransport> outgoingTransport) -> void
    {
        outgoing = outgoingTransport;
    },
        []() -> void
    {
    });

    int limit = 16;
    while (network->update() && --limit > 0)
    {
        if (incoming && outgoing)
        {
            ProcessTransport(outgoing.get(), "Client");
            ProcessTransport(incoming.get(), "Server");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (incoming)
    {
        incoming->disconnect();
    }
    if (outgoing)
    {
        outgoing->disconnect();
    }
    connection_serv->stop();
    connection_client->stop();
}
