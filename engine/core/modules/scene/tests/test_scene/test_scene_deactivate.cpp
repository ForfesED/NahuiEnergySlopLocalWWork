// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/serialization/json_utils.h"
#include "scene_test_base.h"
#include "scene_test_components.h"

namespace nau::test
{
    /**
     */
    class TestSceneDeactivate : public SceneTestBase
    {
    };

    /**
        Test:
     */
    TEST_F(TestSceneDeactivate, SimpleDeactivateScene)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            ObjectWeakRef object = scene->getRoot().attachChild(createObject<scene_test::MyDefaultSceneComponent>());

            ObjectWeakRef sceneRef = co_await getSceneManager().activateScene(std::move(scene));
            co_await skipFrames(2);

            getSceneManager().deactivateScene(sceneRef);

            // expected all external references are going to be invalidate
            ASSERT_FALSE_ASYNC(sceneRef);
            ASSERT_FALSE_ASYNC(object);
            ASSERT_ASYNC(getSceneManager().getActiveScenes().empty());

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
     */
    TEST_F(TestSceneDeactivate, ComponentEventsDuringSceneDeactivation)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            constexpr size_t ObjectsCount = 10;
            constexpr size_t ComponentsPerObjectCount = 10;
            constexpr size_t ExpectedCounter = ObjectsCount * ComponentsPerObjectCount;

            IScene::Ptr scene = createEmptyScene();

            size_t destructorCounter = 0;
            size_t disposedCounter = 0;
            size_t destroyedCounter = 0;
            size_t deactivatedCounter = 0;

            const auto setupComponent = [&](MyDisposableComponent& component)
            {
                component.setOnDeactivated([&]
                {
                    ++deactivatedCounter;
                });

                component.setOnDestroyed([&]
                {
                    ++destroyedCounter;
                });

                component.setOnDisposed([&]
                {
                    ++disposedCounter;
                });

                component.setOnDestructor([&]
                {
                    ++destructorCounter;
                });
            };

            Vector<ObjectWeakRef<SceneObject>> objectRefs;
            objectRefs.reserve(ObjectsCount);

            Vector<ObjectWeakRef<MyDisposableComponent>> componentRefs;
            componentRefs.reserve(ObjectsCount * ComponentsPerObjectCount);

            for (size_t i = 0; i < ObjectsCount; ++i)
            {
                ObjectWeakRef object = scene->getRoot().attachChild(createObject());
                objectRefs.push_back(object);

                for (size_t j = 0; j < ComponentsPerObjectCount; ++j)
                {
                    ObjectWeakRef component = object->addComponent<MyDisposableComponent>();
                    componentRefs.push_back(component);
                    setupComponent(*component);
                }
            }

            ObjectWeakRef sceneRef = co_await getSceneManager().activateScene(std::move(scene));
            co_await skipFrames(2);

            getSceneManager().deactivateScene(sceneRef);

            const bool allObjectsAreInvalidated = std::all_of(objectRefs.begin(), objectRefs.end(), [](const auto& ref)
            {
                return !ref;
            });

            const bool allComponentsAreInvalidated = std::all_of(componentRefs.begin(), componentRefs.end(), [](const auto& ref)
            {
                return !ref;
            });

            ASSERT_ASYNC(allObjectsAreInvalidated)
            ASSERT_ASYNC(allComponentsAreInvalidated)
            ASSERT_ASYNC(destructorCounter == ExpectedCounter);
            ASSERT_ASYNC(disposedCounter == ExpectedCounter);
            ASSERT_ASYNC(deactivatedCounter == ExpectedCounter);

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

  /**
        Test:
     */
    TEST_F(TestSceneDeactivate, ComponentAsyncActivation)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            IScene::WeakRef sceneRef = *scene;
            ObjectWeakRef object = scene->getRoot().attachChild(createObject<MyDefaultSceneComponent>());
            ObjectWeakRef component = object->findFirstComponent<MyDefaultSceneComponent>();
            ObjectWeakRef disposableComponent = object->addComponent<MyDisposableComponent>();

            {  // making component activation asynchronous operation, check that scene is not active (until it actually activated)
                component->setBlockActivation(true);

                auto activateSceneTask = getSceneManager().activateScene(std::move(scene));
                ASSERT_FALSE_ASYNC(activateSceneTask.isReady())
                ASSERT_FALSE_ASYNC(getSceneManager().getActiveScenes().empty());

                component->setBlockActivation(false);
                co_await activateSceneTask;
            }

            co_await skipFrames(2);

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }


    /**
        Test:
     */
    TEST_F(TestSceneDeactivate, ComponentAsyncDeactivation)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            IScene::WeakRef sceneRef = *scene;
            ObjectWeakRef object = scene->getRoot().attachChild(createObject<MyDefaultSceneComponent>());
            ObjectWeakRef component = object->findFirstComponent<MyDefaultSceneComponent>();
            ObjectWeakRef disposableComponent = object->addComponent<MyDisposableComponent>();

            co_await getSceneManager().activateScene(std::move(scene));
            
            component->setBlockDeletion(true);
            MyDefaultSceneComponent* componentRawPtr = component.get();

            getSceneManager().deactivateScene(sceneRef);
            ASSERT_ASYNC(getSceneManager().getActiveScenes().empty());
            ASSERT_FALSE_ASYNC(object);
            ASSERT_FALSE_ASYNC(component);
            ASSERT_FALSE_ASYNC(disposableComponent);

            ASSERT_ASYNC(componentRawPtr->isDeactivated());

            componentRawPtr->setBlockDeletion(false);

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }


    TEST_F(TestSceneDeactivate, DeactivateFromUpdateAndWait)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;
        using namespace nau::scene_test;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            ObjectWeakRef objectRef = scene->getRoot().attachChild(createObject<MyCustomUpdateAction>());
            ObjectWeakRef componentRef = *scene->getRoot().findFirstComponent<MyCustomUpdateAction>(true);

            TaskSource<> signalSource;
            Task<> signal = signalSource.getTask();

            componentRef->setUpdateAsyncCallback([signalSource = std::move(signalSource)](SceneObject&) mutable -> Task<>
            {
                ObjectWeakRef sceneRef = getSceneManager().getActiveScenes().front();
                getSceneManager().deactivateScene(sceneRef);
                NAU_FATAL(!sceneRef);
                signalSource.resolve();

                co_return;
            });

            IScene::WeakRef sceneRef = co_await getSceneManager().activateScene(std::move(scene));
            //co_await skipFrames(1);
            co_await signal;

            ASSERT_FALSE_ASYNC(sceneRef);
            ASSERT_FALSE_ASYNC(objectRef);
            ASSERT_FALSE_ASYNC(componentRef);

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

}  // namespace nau::test
