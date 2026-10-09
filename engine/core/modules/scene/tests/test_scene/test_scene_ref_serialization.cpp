// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/scene_query.h"
#include "nau/string/string_conv.h"
#include "scene_test_base.h"

namespace nau::test
{
    namespace
    {
        class MyComponent1 : public scene::SceneComponent
        {
            NAU_OBJECT(nau::test::MyComponent1, scene::SceneComponent)
            NAU_DECLARE_DYNAMIC_OBJECT
        };

        class MyComponent2 : public scene::SceneComponent
        {
            NAU_OBJECT(nau::test::MyComponent2, scene::SceneComponent)
            NAU_DECLARE_DYNAMIC_OBJECT
            NAU_CLASS_FIELDS(
                CLASS_NAMED_FIELD(m_objectRef, "objectRef"),
                CLASS_NAMED_FIELD(m_componentRef, "componentRef"))

        public:
            void setObjectRef(scene::SceneObject::WeakRef objectRef)
            {
                value_changes_scope;
                m_objectRef = objectRef;
            }

            auto getObjectRef() const
            {
                return m_objectRef;
            }

            void setComponentRef(scene::ObjectWeakRef<MyComponent1> componentRef)
            {
                value_changes_scope;
                m_componentRef = componentRef;
            }

            auto getComponentRef() const
            {
                return m_componentRef;
            }

        private:
            scene::SceneObject::WeakRef m_objectRef;
            scene::ObjectWeakRef<MyComponent1> m_componentRef;
        };

        NAU_IMPLEMENT_DYNAMIC_OBJECT(nau::test::MyComponent1)
        NAU_IMPLEMENT_DYNAMIC_OBJECT(nau::test::MyComponent2)
    }  // namespace

    class TestSceneRefSerialization : public SceneTestBase
    {
    protected:
        static scene::IScene::Ptr createSimpleScene()
        {
            using namespace nau::scene;

            IScene::Ptr scene = createEmptyScene();
            auto& obj1 = scene->getRoot().attachChild(createObject<MyComponent1>("Object_1"));
            auto& obj2 = scene->getRoot().attachChild(createObject("Object_2"));
            obj2.addComponent<MyComponent2>().setObjectRef(obj1);

            auto& obj1_2 = obj1.attachChild(createObject<MyComponent2>("Child_12"));
            MyComponent2& component = obj1_2.getRootComponent<MyComponent2>();
            component.setComponentRef(obj1.getRootComponent<MyComponent1>());
            component.setObjectRef(obj2);

            return scene;
        }

        static void checkSimpleSceneLayout(scene::SceneObject& root)
        {
            using namespace nau::scene;

            auto objects = root.getDirectChildObjects();
            ASSERT_GT(objects.size(), 1);

            // check the reference values (see createSimpleScene for more detail scene structure)
            {
                SceneObject* const object1 = objects[1];

                auto objectRef = object1->findFirstComponent<MyComponent2>()->getObjectRef();
                ASSERT_TRUE(objectRef);
                ASSERT_EQ(objectRef.get(), objects[0]);

                // createSimpleScene will not set any reference here, so it must remains null
                auto componentRef = objects[1]->findFirstComponent<MyComponent2>()->getComponentRef();
                ASSERT_FALSE(componentRef);
            }

            {
                SceneObject* const child1 = objects[0]->getDirectChildObjects().front();

                auto componentRef = child1->getRootComponent<MyComponent2>().getComponentRef();
                ASSERT_TRUE(componentRef);
                ASSERT_EQ(componentRef.get(), &objects[0]->getRootComponent<MyComponent1>());

                auto objectRef = child1->getRootComponent<MyComponent2>().getObjectRef();
                ASSERT_TRUE(objectRef);
                ASSERT_EQ(objectRef.get(), objects[1]);
            }
        }

    private:
        void initializeApp() override
        {
            SceneTestBase::initializeApp();
            registerClasses<MyComponent1,
                            MyComponent2>();
        }
    };

    /**
        Test:
            - Save scene with component with empty references.
            - Load scene, check it loaded success, references are remains empty
    */
    TEST_F(TestSceneRefSerialization, EmptyReferences)
    {
        using namespace nau::scene;

        IScene::Ptr scene = createEmptyScene();

        scene->getRoot()
            .attachChild(createObject("Object_1"))
            .addComponent<MyComponent2>();

        IScene::Ptr sceneCopy = copySceneThroughStream(*scene, {});
        ASSERT_TRUE(scenesEqualSimple(*scene, *sceneCopy));

        SceneObject* const obj1 = sceneCopy->getRoot().getDirectChildObjects().front();
        MyComponent2* component = obj1->findFirstComponent<MyComponent2>();

        ASSERT_EQ(component->getComponentRef().get(), nullptr);
        ASSERT_EQ(component->getObjectRef().get(), nullptr);
    }

    /**
        Test:
            - Create scene with simple structure
            - Add some components that can save references
            - Set references to scene object and components
            - Save scene
            - Load scene (with no uids recreation)
            - Check scene structure
            - Check references are linked to the valid/expected objects
     */
    TEST_F(TestSceneRefSerialization, CopyScene)
    {
        using namespace nau::io;
        using namespace nau::scene;

        IScene::Ptr scene = createSimpleScene();
        IScene::Ptr sceneCopy = copySceneThroughStream(*scene, {});

        ASSERT_TRUE(scenesEqualSimple(*scene, *sceneCopy, true));
        ASSERT_NO_FATAL_FAILURE(checkSimpleSceneLayout(sceneCopy->getRoot()));
    }

    /**
        Test:
        - Create and save scene
        - Load scene as copy (with uids recreation)
        - Check scene structure
        - Check references are linked to the valid/expected objects
     */
    TEST_F(TestSceneRefSerialization, CopyScene_RecreateUids)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::io;
        using namespace nau::scene;

        IScene::Ptr scene = createSimpleScene();
        IScene::Ptr sceneCopy = copySceneThroughStream(*scene, CreateSceneOption::RecreateUid);

        ASSERT_FALSE(scenesEqualSimple(*scene, *sceneCopy, true));
        ASSERT_TRUE(scenesEqualSimple(*scene, *sceneCopy, false));
        ASSERT_NO_FATAL_FAILURE(checkSimpleSceneLayout(sceneCopy->getRoot()));
    }

    /**
        Test:
        - Create scene and save scene as asset.
        - Load prefab asset and makes its copy
        - Check object structure
        - Check references are linked to the valid/expected objects
     */
    TEST_F(TestSceneRefSerialization, Prefab_Instantiation)
    {
        using namespace nau::scene;

        IScene::Ptr scene = createSimpleScene();
        SceneObject::Ptr prefabInstance = copySceneObjectThroughStream(scene->getRoot(), CreateSceneOption::RecreateUid);

        ASSERT_FALSE(sceneObjectsEqualSimple(scene->getRoot(), *prefabInstance, true));
        ASSERT_TRUE(sceneObjectsEqualSimple(scene->getRoot(), *prefabInstance, false));
        ASSERT_NO_FATAL_FAILURE(checkSimpleSceneLayout(*prefabInstance));
    }

    /**
        Test: References to external objects are resolved as null (unless these objects are activated globally)
            - create scene (don not activate it)
            - create an object that has references (from its component) to a previously created scene objects
            - instantiate prefab (create copy)
            - validate that instance have valid structure
            - validate that instance's component references are null (because references cannot be resolved from the instance itself or active scenes)
     */
    TEST_F(TestSceneRefSerialization, UnresolvableExternalReferences)
    {
        using namespace nau::scene;

        IScene::Ptr scene = createSimpleScene();
        SceneObject& objectRefValue = scene->getRoot();
        MyComponent1& componentRefValue = scene->getRoot().getDirectChildObjects().front()->getRootComponent<MyComponent1>();

        auto prefab = EXPR_Block->SceneObject::Ptr
        {
            auto root = createObject("Object_1");
            auto& child = root->attachChild(createObject("Child_1"));
            auto& component = child.addComponent<MyComponent2>();

            component.setObjectRef(objectRefValue);
            component.setComponentRef(componentRefValue);

            return root;
        };

        SceneObject::Ptr instance = copySceneObjectThroughStream(*prefab, CreateSceneOption::RecreateUid);
        ASSERT_TRUE(sceneObjectsEqualSimple(*prefab, *instance, false));

        MyComponent2* const component = instance->getDirectChildObjects().front()->findFirstComponent<MyComponent2>();
        ASSERT_EQ(component->getComponentRef().get(), nullptr);
        ASSERT_EQ(component->getObjectRef().get(), nullptr);
    }

    /**
        Test: References to external objects are correctly resolved to objects/components when these objects are activated globally
            - create scene and activate it
            - create an object that has references (from its component) to a previously created scene objects
            - instantiate prefab (create copy)
            - validate that instance have valid structure
            - validate that instance's component references are resolved to global objects from active scene
     */
    TEST_F(TestSceneRefSerialization, ExternalReferences)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([]() -> Task<AssertionResult>
        {
            ObjectWeakRef scene = co_await getSceneManager().activateScene(createSimpleScene());

            SceneObject& objectRefValue = scene->getRoot();
            MyComponent1& componentRefValue = scene->getRoot().getDirectChildObjects().front()->getRootComponent<MyComponent1>();

            auto prefab = EXPR_Block->SceneObject::Ptr
            {
                auto root = createObject("Object_1");
                auto& child = root->attachChild(createObject("Child_1"));
                auto& component = child.addComponent<MyComponent2>();

                component.setObjectRef(objectRefValue);
                component.setComponentRef(componentRefValue);

                return root;
            };

            SceneObject::Ptr instance = copySceneObjectThroughStream(*prefab, CreateSceneOption::RecreateUid);
            ASSERT_ASYNC(sceneObjectsEqualSimple(*prefab, *instance, false));

            MyComponent2* const component = instance->getDirectChildObjects().front()->findFirstComponent<MyComponent2>();
            ASSERT_ASYNC(component->getComponentRef().get() == &componentRefValue);
            ASSERT_ASYNC(component->getObjectRef().get() == &objectRefValue);

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

}  // namespace nau::test
