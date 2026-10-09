// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/components/component_life_cycle.h"
#include "nau/service/service_provider.h"
#include "scene_test_base.h"

namespace nau::test
{
    namespace
    {
        class MyTestComponent : public scene::SceneComponent,
                                public scene::IComponentEvents
        {
            NAU_COMPONENT(test::MyTestComponent, scene::SceneComponent, scene::IComponentEvents)

        public:
            bool onComponentCreatedCalledOnce() const
            {
                return (m_onCreatedCalledCounter == 1);
            }

        private:
            void onComponentCreated() override
            {
                // expected that component at this moment is operable (i.e. attached to the scene).
                NAU_FATAL(isOperable());
                ++m_onCreatedCalledCounter;
            }

            size_t m_onCreatedCalledCounter = 0;
        };

        NAU_IMPLEMENT_COMPONENT(MyTestComponent)
    }  // namespace

    class TestComponentEvents : public SceneTestBase
    {
    protected:
        void initializeApp() override
        {
            registerClasses<MyTestComponent>();
        }
    };

    /**
        Test: calling onComponentCreated for non root component
     */
    TEST_F(TestComponentEvents, OnCreated)
    {
        using namespace nau::scene;

        auto scene = createEmptyScene();
        scene->getRoot().addComponent<MyTestComponent>();
        ASSERT_TRUE(scene->getRoot().findFirstComponent<MyTestComponent>()->onComponentCreatedCalledOnce());
    }

    /**
        Test: calling onComponentCreated for root component
     */
    TEST_F(TestComponentEvents, OnCreated_ForRoot)
    {
        using namespace nau::scene;

        SceneObject::Ptr objectPtr = createObject<MyTestComponent>();
        ASSERT_TRUE(objectPtr->getRootComponent<MyTestComponent>().onComponentCreatedCalledOnce());
    }

    /**
        Test: calling onComponentCreated while serialization
     */
    TEST_F(TestComponentEvents, OnCreated_OnSerialization)
    {
        using namespace nau::scene;

        io::IMemoryStream::Ptr memStream = EXPR_Block
        {
            IScene::Ptr scene = createEmptyScene();
            scene->getRoot().addComponent<MyTestComponent>();
            scene->getRoot().attachChild(createObject<MyTestComponent>());
            return dumpSceneToMemoryStream(*scene);
        };

        memStream->setPosition(io::OffsetOrigin::Begin, 0);
        IScene::Ptr scene2 = restoreSceneFromStream(*memStream, {});
        ASSERT_TRUE(scene2->getRoot().findFirstComponent<MyTestComponent>(false)->onComponentCreatedCalledOnce());
        ASSERT_TRUE(scene2->getRoot().getDirectChildObjects().front()->getRootComponent<MyTestComponent>().onComponentCreatedCalledOnce());
    }

}  // namespace nau::test
