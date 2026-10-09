// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <EASTL/shared_ptr.h>

#include "../src/net_connector_impl.h"
#include "../src/networking_factory_impl.h"
#include "nau/asio/networking_asio.h"

namespace nau
{
    namespace test
    {
        TEST(TestNetConnector, TestTemplate)
        {
            ASSERT_TRUE(true);
        }

        TEST(TestNetConnector, TestCreate)
        {
            eastl::shared_ptr<NetConnectorImpl> netConnector = eastl::make_shared<NetConnectorImpl>();
            ASSERT_TRUE(netConnector);
        }

        TEST(TestNetConnector, TestConnectionSingle)
        {
            NetworkingFactoryImpl::Register("ASIO", NetworkingASIO::create);
            auto netPtr = NetworkingFactoryImpl::Create("ASIO");
            ASSERT_TRUE(netPtr);
            eastl::shared_ptr<NetConnectorImpl> netConnector = eastl::make_shared<NetConnectorImpl>();
            ASSERT_TRUE(netConnector);
            eastl::string peerId1("Peer1");
            eastl::string peerId2("Peer2");
            eastl::string url("tcp://127.0.0.1:9995/");
            netConnector->init(netPtr);
            netConnector->listen(peerId1, peerId2, url);
            netConnector->update();
            netConnector->connect(peerId2, peerId1, url);
            netConnector->update();
            netConnector->update();
            netConnector->update();
            netConnector->update();
            eastl::vector<eastl::string> peers;
            netConnector->getConnections(peerId1, peers);
            ASSERT_TRUE(peers.size() == 1);
            ASSERT_TRUE(peers[0] == peerId2);
            netConnector->getConnections(peerId2, peers);
            ASSERT_TRUE(peers.size() == 1);
            ASSERT_TRUE(peers[0] == peerId1);
        }

        bool containPeers(eastl::vector<eastl::string>& peers, eastl::set<eastl::string> originals)
        {
            for (auto& peer : peers)
            {
                auto it = originals.find(peer);
                if (it == originals.end())
                {
                    return false;
                }
                else
                {
                    originals.erase(peer);
                }
            }
            return true;
        }

        TEST(TestNetConnector, TestConnectionMulitple)
        {
            NetworkingFactoryImpl::Register("ASIO", NetworkingASIO::create);
            auto netPtr = NetworkingFactoryImpl::Create("ASIO");
            ASSERT_TRUE(netPtr);
            eastl::shared_ptr<NetConnectorImpl> netConnector = eastl::make_shared<NetConnectorImpl>();
            ASSERT_TRUE(netConnector);
            eastl::string peerId0("Peer1");
            eastl::string peerId1("Peer2");
            eastl::string peerId2("Peer3");
            eastl::string peerId3("Peer4");
            eastl::string url1("tcp://127.0.0.1:9990/");
            eastl::string url2("tcp://127.0.0.1:9991/");
            eastl::string url3("tcp://127.0.0.1:9992/");
            netConnector->init(netPtr);
            netConnector->listen(peerId0, peerId1, url1);
            netConnector->listen(peerId0, peerId2, url2);
            netConnector->listen(peerId0, peerId3, url3);
            netConnector->update();
            netConnector->connect(peerId1, peerId0, url1);
            netConnector->connect(peerId2, peerId0, url2);
            netConnector->connect(peerId3, peerId0, url3);
            netConnector->update();
            netConnector->update();
            netConnector->update();
            netConnector->update();
            eastl::vector<eastl::string> peers;
            netConnector->getConnections(peerId0, peers);
            ASSERT_TRUE(peers.size() == 3);
            ASSERT_TRUE(containPeers(peers, {peerId1, peerId2, peerId3}));
            netConnector->getConnections(peerId1, peers);
            ASSERT_TRUE(peers.size() == 1);
            ASSERT_TRUE(peers[0] == peerId0);
            netConnector->getConnections(peerId2, peers);
            ASSERT_TRUE(peers.size() == 1);
            ASSERT_TRUE(peers[0] == peerId0);
            netConnector->getConnections(peerId3, peers);
            ASSERT_TRUE(peers.size() == 1);
            ASSERT_TRUE(peers[0] == peerId0);
        }

        TEST(TestNetConnector, TestConnectionMesh3)
        {
            NetworkingFactoryImpl::Register("ASIO", NetworkingASIO::create);
            auto netPtr = NetworkingFactoryImpl::Create("ASIO");
            ASSERT_TRUE(netPtr);
            eastl::shared_ptr<NetConnectorImpl> netConnector = eastl::make_shared<NetConnectorImpl>();
            ASSERT_TRUE(netConnector);
            eastl::string peerId1("Peer1");
            eastl::string peerId2("Peer2");
            eastl::string peerId3("Peer3");
            eastl::string url1("tcp://127.0.0.1:9980/");
            eastl::string url2("tcp://127.0.0.1:9981/");
            eastl::string url3("tcp://127.0.0.1:9982/");
            netConnector->init(netPtr);
            netConnector->listen(peerId1, peerId2, url1);
            netConnector->listen(peerId1, peerId3, url2);
            netConnector->listen(peerId2, peerId3, url3);
            netConnector->update();
            netConnector->connect(peerId2, peerId1, url1);
            netConnector->connect(peerId3, peerId1, url2);
            netConnector->connect(peerId3, peerId2, url3);
            netConnector->update();
            netConnector->update();
            netConnector->update();
            netConnector->update();
            eastl::vector<eastl::string> peers;
            netConnector->getConnections(peerId1, peers);
            ASSERT_TRUE(peers.size() == 2);
            // ASSERT_TRUE(containPeers(peers, {peerId1, peerId2, peerId3}));
            netConnector->getConnections(peerId2, peers);
            ASSERT_TRUE(peers.size() == 2);
            netConnector->getConnections(peerId3, peers);
            ASSERT_TRUE(peers.size() == 2);
        }

    }  // namespace test
}  // namespace nau
