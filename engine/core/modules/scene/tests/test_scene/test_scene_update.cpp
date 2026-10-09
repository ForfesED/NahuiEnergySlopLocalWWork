// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/components/component_life_cycle.h"
#include "nau/scene/scene_processor.h"
#include "scene_test_base.h"
#include "scene_test_components.h"

namespace nau::test
{

    /**
     */
    class TestSceneUpdate : public SceneTestBase
    {
    };

    /**
        Test:
            - scene is activated
            - wait some frames
            - check that update called expected times count
     */
    TEST_F(TestSceneUpdate, ComponentUpdate)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            constexpr unsigned FrameCount = 2;
            IScene::Ptr scene = createEmptyScene();
            auto& child = scene->getRoot().attachChild(createObject<scene_test::MyDefaultSceneComponent>());

            co_await getSceneManager().activateScene(std::move(scene));
            co_await skipFrames(FrameCount);

            auto& component = child.getRootComponent<scene_test::MyDefaultSceneComponent>();
            ASSERT_ASYNC(component.getUpdateCounter() == FrameCount);

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test:
            - scene is activated
            - after scene activated component with update and component with async-updated are added into the scene
            - check that update called expected times count
            - check that update async also called but less often (because asynchronous update should not block the execution of the main/scene thread)
     */
    TEST_F(TestSceneUpdate, ComponentAsyncUpdate)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();

            ObjectWeakRef sceneRef = co_await getSceneManager().activateScene(std::move(scene));
            co_await skipFrames(1);

            ObjectWeakRef updateComponent = co_await sceneRef->getRoot().addComponentAsync<scene_test::MyDefaultSceneComponent>();
            ObjectWeakRef asyncUpdateComponent = co_await sceneRef->getRoot().addComponentAsync<scene_test::MyComponentWithAsyncUpdate>();
            asyncUpdateComponent->setAwaitTime(0ms);
            
            // forcing AsyncUpdate executing during multiple frames.
            asyncUpdateComponent->setBlockAsyncUpdate(true);

            constexpr size_t SkipFrames = 3;
            co_await skipFrames(SkipFrames);

            asyncUpdateComponent->setBlockAsyncUpdate(false);

            ASSERT_ASYNC(asyncUpdateComponent->getUpdateAsyncCounter() != SkipFrames);
            ASSERT_ASYNC(updateComponent->getUpdateCounter() == SkipFrames);

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
     */
    TEST_F(TestSceneUpdate, AddComponentFromUpdate)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;
        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();

            ObjectWeakRef object = scene->getRoot().attachChild(createObject<MyCustomUpdateAction>());

            object->getRootComponent<MyCustomUpdateAction>().setUpdateAsyncCallback([&](SceneObject& object) -> Task<>
            {
                co_await object.addComponentAsync<MyComponentWithAsyncUpdate>();
                co_await object.addComponentAsync<MyDefaultSceneComponent>();
            });

            co_await getSceneManager().activateScene(std::move(scene));
            co_await skipFrames(2);

            auto* component1 = object->findFirstComponent<scene_test::MyDefaultSceneComponent>();
            auto* component2 = object->findFirstComponent<scene_test::MyComponentWithAsyncUpdate>();

            ASSERT_ASYNC(component1->getUpdateCounter() > 0)
            ASSERT_ASYNC(component2->getUpdateAsyncCounter() > 0)

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
     */
    TEST_F(TestSceneUpdate, AddObjectFromUpdate)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;
        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();

            auto& object = scene->getRoot().attachChild(createObject<MyCustomUpdateAction>());
            object.getRootComponent<MyCustomUpdateAction>().setUpdateAsyncCallback([](SceneObject& object) -> Task<>
            {
                auto newObject = getServiceProvider().get<ISceneFactory>().createSceneObject();
                newObject->addComponent<scene_test::MyComponentWithAsyncUpdate>();
                newObject->addComponent<scene_test::MyDefaultSceneComponent>();

                co_await object.attachChildAsync(std::move(newObject));
            });

            co_await getSceneManager().activateScene(std::move(scene));
            co_await skipFrames(2);

            auto* addedObject = object.getDirectChildObjects().front();
            auto* component1 = addedObject->findFirstComponent<scene_test::MyDefaultSceneComponent>();
            auto* component2 = addedObject->findFirstComponent<scene_test::MyComponentWithAsyncUpdate>();

            ASSERT_ASYNC(component1->getUpdateCounter() > 0)
            ASSERT_ASYNC(component2->getUpdateAsyncCounter() > 0)

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test:
            Removing the some component while update is processed.
     */
    TEST_F(TestSceneUpdate, RemoveComponentFromUpdate)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            size_t destructorCounter = 0;
            size_t disposedCounter = 0;
            size_t destroyedCounter = 0;
            size_t deactivatedCounter = 0;

            IScene::Ptr scene = createEmptyScene();

            auto& object = scene->getRoot().attachChild(createObject<MyCustomUpdateAction>());

            ObjectWeakRef component = object.addComponent<MyDisposableComponent>();
            component->setOnDeactivated([&]
            {
                ++deactivatedCounter;
            });

            component->setOnDestroyed([&]
            {
                ++destroyedCounter;
            });

            component->setOnDisposed([&]
            {
                ++disposedCounter;
            });

            component->setOnDestructor([&]
            {
                ++destructorCounter;
            });

            object.getRootComponent<MyCustomUpdateAction>().setUpdateAsyncCallback([](SceneObject& object) -> Task<>
            {
                ObjectWeakRef component = *object.findFirstComponent<MyDisposableComponent>();
                object.removeComponent(component);

                // Even though the component is deleted inside the update call, all references to it will be immediately invalidated.
                // But the actual deletion of the component will only be performed after exiting the current update loop.
                NAU_FATAL(!component);

                return makeResolvedTask();
            });
            co_await getSceneManager().activateScene(std::move(scene));

            co_await skipFrames(2);

            ASSERT_ASYNC(object.findFirstComponent<MyDisposableComponent>() == nullptr)
            ASSERT_ASYNC(destructorCounter == 1)
            ASSERT_ASYNC(disposedCounter == 1)
            // ASSERT_ASYNC(destroyedCounter == componentCounter)
            ASSERT_ASYNC(deactivatedCounter == 1)

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test:
            Removing a component for which an update is called
     */
    // TODO NAU-2089
    /*
    TEST_F(TestSceneUpdate, RemoveThisComponentFromUpdate)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();

            auto& object = scene->getRoot().attachChild(createObject());

            ObjectWeakRef componentRef = object.addComponent<MyCustomUpdateAction>();
            componentRef->setUpdateAsyncCallback([](SceneObject& object) -> Task<>
            {
                ObjectWeakRef componentSelfReference = *object.findFirstComponent<MyCustomUpdateAction>();
                componentSelfReference->destroy();

                // Expected that all external references will still be valid while update is processed.
                NAU_FATAL(componentSelfReference);
                NAU_FATAL(object.findFirstComponent<MyCustomUpdateAction>() != nullptr);

                // making some async action
                co_await 1ms;
            });

            co_await getSceneManager().activateScene(std::move(scene));
            co_await 10ms;

            ASSERT_ASYNC(object.findFirstComponent<MyCustomUpdateAction>() == nullptr)
            ASSERT_FALSE_ASYNC(componentRef)
            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }
    */

    /**
        Test:
            Destroying an object for which an update is called
     */
    TEST_F(TestSceneUpdate, DestroyObjectFromUpdate)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        using namespace std::chrono_literals;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            size_t destructorCounter = 0;
            size_t disposedCounter = 0;
            size_t destroyedCounter = 0;
            size_t deactivatedCounter = 0;

            IScene::Ptr scene = createEmptyScene();

            ObjectWeakRef object = scene->getRoot().attachChild(createObject<MyCustomUpdateAction>());

            ObjectWeakRef component = object->addComponent<MyDisposableComponent>();
            component->setOnDeactivated([&]
            {
                ++deactivatedCounter;
            });

            component->setOnDestroyed([&]
            {
                ++destroyedCounter;
            });

            component->setOnDisposed([&]
            {
                ++disposedCounter;
            });

            component->setOnDestructor([&]
            {
                ++destructorCounter;
            });

            object->getRootComponent<MyCustomUpdateAction>().setUpdateAsyncCallback([](SceneObject& object) -> Task<>
            {
                ObjectWeakRef objectRef = object;
                object.destroy();
                NAU_FATAL(!objectRef);

                return makeResolvedTask();
            });
            co_await getSceneManager().activateScene(std::move(scene));

            co_await skipFrames(2);

            ASSERT_ASYNC(!object)
            ASSERT_ASYNC(destructorCounter == 1)
            ASSERT_ASYNC(disposedCounter == 1)
            // ASSERT_ASYNC(destroyedCounter == componentCounter)
            ASSERT_ASYNC(deactivatedCounter == 1)

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

}  // namespace nau::test
