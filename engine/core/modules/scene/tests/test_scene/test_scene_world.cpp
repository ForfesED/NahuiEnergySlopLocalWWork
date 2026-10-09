// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/world.h"
#include "scene_test_base.h"
#include "scene_test_components.h"

namespace nau::test
{
    class TestSceneWorld : public SceneTestBase
    {
    protected:
        static async::Task<scene::IScene::WeakRef> createEmptySceneInWorld(scene::IWorld::WeakRef world)
        {
            scene::IScene::Ptr scene = createEmptyScene();
            co_return (co_await world->addScene(std::move(scene)));
        }
    };

    /**
        Test:
            Default world is accessible by default
    */
    TEST_F(TestSceneWorld, DefaultWorldExists)
    {
        scene::IWorld& defaultWorld = getSceneManager().getDefaultWorld();
        ASSERT_TRUE(defaultWorld.getScenes().empty());
        ASSERT_EQ(getSceneManager().getWorlds().size(), 1);
        ASSERT_EQ(getSceneManager().getWorlds().front().get(), &defaultWorld);
    }

    /**
        Test:
            The inactive does not scene have a world
    */
    TEST_F(TestSceneWorld, NonActiveSceneHasNoWorld)
    {
        scene::ObjectUniquePtr scene = createEmptyScene();
        ASSERT_EQ(scene->getWorld(), nullptr);
    }

    /**
        Test:
            Validate ISceneManager::createWorld() and checks worlds collection
     */
    TEST_F(TestSceneWorld, CreateWorld)
    {
        using namespace nau::scene;

        ObjectWeakRef world = getSceneManager().createWorld();
        ASSERT_TRUE(world);
        ASSERT_EQ(getSceneManager().getWorlds().size(), 2);
    }

    /**
        Test:
            After world deletion the world's reference becomes invalid
     */
    TEST_F(TestSceneWorld, DeleteWorld)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IWorld::WeakRef world = getSceneManager().createWorld();
            ASSERT_ASYNC(world);

            getSceneManager().destroyWorld(world);

            ASSERT_FALSE_ASYNC(world)
            ASSERT_ASYNC(getSceneManager().getWorlds().size() == 1)

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test:
            Create scene within world, check
                - active scenes collection does not contains scene (active scenes belong to default world)
                - created world contains scene
     */
    TEST_F(TestSceneWorld, CreateSceneInWorld)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IWorld::WeakRef world = getSceneManager().createWorld();
            ObjectWeakRef sceneRef = co_await createEmptySceneInWorld(world);

            ASSERT_ASYNC(sceneRef)
            ASSERT_ASYNC(sceneRef->getWorld() == world.get())
            ASSERT_ASYNC(getSceneManager().getActiveScenes().empty())
            ASSERT_FALSE_ASYNC(world->getScenes().empty())
            ASSERT_ASYNC(sceneRef->getWorld() == world.get())

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test:
            Create and delete scene through IWorld::removeScene.
     */
    TEST_F(TestSceneWorld, DeleteSceneInWorld)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IWorld::WeakRef world = getSceneManager().createWorld();
            ObjectWeakRef sceneRef = co_await createEmptySceneInWorld(world);

            world->removeScene(sceneRef);

            ASSERT_FALSE_ASYNC(sceneRef)
            ASSERT_ASYNC(world->getScenes().empty())

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test:
            Create scene in world and deactivate through ISceneManager::deactivateScene
     */
    TEST_F(TestSceneWorld, DeactivateScene)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IWorld::WeakRef world = getSceneManager().createWorld();
            ObjectWeakRef sceneRef = co_await createEmptySceneInWorld(world);

            getSceneManager().deactivateScene(sceneRef);

            ASSERT_FALSE_ASYNC(sceneRef)
            ASSERT_ASYNC(world->getScenes().empty())

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test:
            Destroy world that contains scene, check scene becomes invalid.
    */
    TEST_F(TestSceneWorld, DeleteWorldContainingScenes)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IWorld::WeakRef world = getSceneManager().createWorld();
            ObjectWeakRef scene0 = co_await createEmptySceneInWorld(world);
            ObjectWeakRef scene1 = co_await createEmptySceneInWorld(world);

            getSceneManager().destroyWorld(world);

            ASSERT_FALSE_ASYNC(world)
            ASSERT_FALSE_ASYNC(scene0)
            ASSERT_FALSE_ASYNC(scene1)

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }
}  // namespace nau::test
