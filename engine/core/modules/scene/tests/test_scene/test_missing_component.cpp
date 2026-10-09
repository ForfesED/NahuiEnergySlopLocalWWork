// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/components/internal/missing_component.h"
#include "nau/test/helpers/app_guard.h"
#include "scene_test_base.h"

namespace nau::test
{
    namespace
    {
        class MyComponent1 : public scene::Component
        {
        public:
            NAU_COMPONENT(MyComponent1, scene::Component)

            NAU_CLASS_FIELDS(
                CLASS_FIELD(field1),
                CLASS_FIELD(field2))

            std::string field1;
            unsigned field2 = 0;
        };

        class MySceneComponent1 : public scene::SceneComponent
        {
            NAU_COMPONENT(MySceneComponent1, scene::SceneComponent)

            NAU_CLASS_FIELDS(
                CLASS_FIELD(field1),
                CLASS_FIELD(field2))

            std::string field1;
            unsigned field2 = 0;
        };

        NAU_IMPLEMENT_COMPONENT(MyComponent1)
        NAU_IMPLEMENT_COMPONENT(MySceneComponent1)

    }  // namespace

    class TestMissingComponent : public testing::Test
    {
    };

    /**
        Test: Missing Component for 'additional' (not root) component.
            When component is not registered the system must create 'missing component' as placeholder.
            However, in this case, such component must be stored (serialized) and subsequently restored as if it were a real component, and not a "placeholder/stub"
    */
    TEST_F(TestMissingComponent, ComponentPlaceholder)
    {
        using namespace nau::scene;

        io::IMemoryStream::Ptr stream;

        {
            // - register component
            // - create scene with component
            // - dump scene to the memory stream
            AppGuard appGuard;
            appGuard.start();
            AppGuard::registerClasses<MyComponent1>();

            auto object1 = SceneTestBase::createObject("Object1");
            auto& component = object1->addComponent<MyComponent1>();
            component.field1 = "text";
            component.field2 = 77;

            auto scene = SceneTestBase::createEmptyScene();
            scene->getRoot().attachChild(std::move(object1));

            stream = SceneTestBase::dumpSceneToMemoryStream(*scene);
            stream->setPosition(io::OffsetOrigin::Begin, 0);
        }

        io::IMemoryStream::Ptr stream2;

        {
            // - DO NOT register components
            // - restore scene from stream (where unregistered component exists)
            // - expect that scene successfully loaded
            // - check that object contains MissingComponent placeholder instead of real component (because it missing)
            // - dump scene to the memory stream2
            AppGuard appGuard;
            appGuard.start();

            IScene::Ptr scene = SceneTestBase::restoreSceneFromStream(*stream, {});

            SceneObject* const object1 = scene->getRoot().getDirectChildObjects().front();
            Component* const component = object1->getDirectComponents()[1];
            ASSERT_TRUE(component->is<IMissingComponent>());

            stream2 = SceneTestBase::dumpSceneToMemoryStream(*scene);
            stream2->setPosition(io::OffsetOrigin::Begin, 0);
        }

        // - register components
        // - restore scene from stream2
        // - expect that scene successfully loaded
        // - check that object contains real component and its data is not lost during previous serialization
        // - dump scene to the memory stream
        AppGuard appGuard;
        appGuard.start();
        AppGuard::registerClasses<MyComponent1>();
        IScene::Ptr scene = SceneTestBase::restoreSceneFromStream(*stream2, {});

        MyComponent1* const component = scene->getRoot().findFirstComponent<MyComponent1>(true);
        ASSERT_TRUE(component);
        ASSERT_EQ(component->field1, "text");
        ASSERT_EQ(component->field2, 77);
    }

    /**
        Test: Missing Component functionality for root component.
    */
    TEST_F(TestMissingComponent, RootComponentPlaceholder)
    {
        using namespace nau::scene;

        io::IMemoryStream::Ptr stream;

        {
            // - register component
            // - create scene with component
            // - dump scene to the memory stream
            AppGuard appGuard;
            appGuard.start();
            AppGuard::registerClasses<MySceneComponent1>();

            auto object1 = SceneTestBase::createObject<MySceneComponent1>("Object1");
            auto& component = object1->getRootComponent<MySceneComponent1>();
            component.field1 = "text";
            component.field2 = 77;

            auto scene = SceneTestBase::createEmptyScene();
            scene->getRoot().attachChild(std::move(object1));

            stream = SceneTestBase::dumpSceneToMemoryStream(*scene);
            stream->setPosition(io::OffsetOrigin::Begin, 0);
        }

        io::IMemoryStream::Ptr stream2;

        {
            // - DO NOT register components
            // - restore scene from stream (where unregistered component exists)
            // - expect that scene successfully loaded
            // - check that object contains MissingComponent placeholder instead of real component (because it missing)
            // - dump scene to the memory stream2
            AppGuard appGuard;
            appGuard.start();

            IScene::Ptr scene = SceneTestBase::restoreSceneFromStream(*stream, {});

            SceneObject* const object1 = scene->getRoot().getDirectChildObjects().front();
            SceneComponent& component = object1->getRootComponent();
            ASSERT_TRUE(component.is<IMissingComponent>());

            stream2 = SceneTestBase::dumpSceneToMemoryStream(*scene);
            stream2->setPosition(io::OffsetOrigin::Begin, 0);
        }

        // - register components
        // - restore scene from stream2
        // - expect that scene successfully loaded
        // - check that object contains real component and its data is not lost during previous serialization
        // - dump scene to the memory stream
        AppGuard appGuard;
        appGuard.start();
        AppGuard::registerClasses<MySceneComponent1>();
        IScene::Ptr scene = SceneTestBase::restoreSceneFromStream(*stream2, {});

        MySceneComponent1* const component = scene->getRoot().findFirstComponent<MySceneComponent1>(true);
        ASSERT_TRUE(component);
        ASSERT_EQ(component->field1, "text");
        ASSERT_EQ(component->field2, 77);
    }

}  // namespace nau::test
