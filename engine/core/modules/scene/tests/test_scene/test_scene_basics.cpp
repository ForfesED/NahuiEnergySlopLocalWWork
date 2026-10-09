// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/utils/functor.h"
#include "scene_test_base.h"
#include "scene_test_components.h"

namespace nau::test
{
    namespace
    {

        /**
         */
        class MyComponent1 : public scene::Component,
                             public scene_test::WithDestructor
        {
            NAU_OBJECT(MyComponent1, scene::Component);
            NAU_DECLARE_DYNAMIC_OBJECT
        };

        /**
         */
        class MyComponent2 : public scene::Component,
                             public scene_test::WithDestructor
        {
            NAU_OBJECT(MyComponent2, scene::Component);
            NAU_DECLARE_DYNAMIC_OBJECT
        };

        /**
         */
        class MySceneComponent1 : public scene::SceneComponent,
                                  public scene_test::WithDestructor
        {
            NAU_OBJECT(MySceneComponent1, scene::SceneComponent);
            NAU_DECLARE_DYNAMIC_OBJECT
            NAU_CLASS_FIELDS(
                CLASS_FIELD(m_value))
        public:
            unsigned getMyValue() const
            {
                return m_value;
            }

            void setMyValue(unsigned value)
            {
                value_changes_scope;
                m_value = value;
            }

        private:
            unsigned m_value = 0;
        };

        NAU_IMPLEMENT_DYNAMIC_OBJECT(MyComponent1);
        NAU_IMPLEMENT_DYNAMIC_OBJECT(MyComponent2);
        NAU_IMPLEMENT_DYNAMIC_OBJECT(MySceneComponent1);
    }  // namespace

    /**
     */
    class TestSceneBasics : public SceneTestBase
    {
    private:
        void initializeApp() override
        {
            registerClasses<
                MyComponent1,
                MyComponent2,
                MySceneComponent1>();
        }
    };

    /**
        Test:
            creating empty scene.
     */
    TEST_F(TestSceneBasics, CreateEmptyScene)
    {
        using namespace nau::scene;

        IScene::Ptr scene = createEmptyScene();

        ASSERT_TRUE(scene);
        SceneObject& root = scene->getRoot();

        ASSERT_EQ(root.getScene(), scene.get());
    }

    /**
        Test:
            creating scene object with custom scene component type.
    */
    TEST_F(TestSceneBasics, ObjectWithCustomSceneComponent)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = getSceneFactory().createSceneObject<MySceneComponent1>();
        ASSERT_TRUE(object->getRootComponent().is<MySceneComponent1>());
        ASSERT_EQ(&object->getRootComponent().getParentObject(), object.get());

        bool componentIsDestructed = false;
        object->getRootComponent<MySceneComponent1>().setOnDestructor([&]
        {
            componentIsDestructed = true;
        });

        object.reset();
        ASSERT_TRUE(componentIsDestructed);
    }

    /**
        Test:
    */
    TEST_F(TestSceneBasics, AttachChild)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = createObject();

        SceneObject::Ptr child1 = createObject("child1");
        SceneObject::Ptr child2 = createObject();

        child1->attachChild(createObject());
        child1->attachChild(createObject());

        object->attachChild(std::move(child1));
        object->attachChild(std::move(child2));

        {
            const auto children = object->getAllChildObjects();
            ASSERT_EQ(children.size(), 4);

            const bool objectIsParent = std::all_of(children.begin(), children.end(), [&object](SceneObject* obj)
            {
                return obj->getParentObject() == object.get();
            });

            ASSERT_FALSE(objectIsParent);
        }

        {
            const auto children = object->getDirectChildObjects();
            ASSERT_EQ(children.size(), 2);

            const bool objectIsParent = std::all_of(children.begin(), children.end(), [&object](SceneObject* obj)
            {
                return obj->getParentObject() == object.get();
            });

            ASSERT_TRUE(objectIsParent);
        }

        object.reset();
    }

    /**
        Test:
     */
    TEST_F(TestSceneBasics, ObjectRemoveChild)
    {
        using namespace nau::scene;

        size_t destructorCounter = 0;

        SceneObject::Ptr object = getSceneFactory().createSceneObject();
        SceneObject::Ptr child1 = getSceneFactory().createSceneObject(&rtti::getTypeInfo<MySceneComponent1>());

        child1->getRootComponent<MySceneComponent1>().setOnDestructor([&]
        {
            ++destructorCounter;
        });

        child1->addComponent<MyComponent1>().setOnDestructor([&]
        {
            ++destructorCounter;
        });

        {
            SceneObject::Ptr child1_1 = getSceneFactory().createSceneObject(&rtti::getTypeInfo<MySceneComponent1>());

            child1_1->addComponent<MyComponent1>().setOnDestructor([&]
            {
                ++destructorCounter;
            });

            child1->attachChild(std::move(child1_1));
        }

        object->attachChild(std::move(child1));

        {
            const auto descendants = object->getDirectChildObjects();
            ASSERT_EQ(descendants.size(), 1);

            SceneObject& childRef = *descendants.front();
            object->removeChild(childRef);
        }

        const auto descendants = object->getDirectChildObjects();
        ASSERT_TRUE(descendants.empty());
        ASSERT_EQ(destructorCounter, 3);
    }

    /**
        Test:
            Check that we can normally call SceneObject::destroy() for object that is not owning by scene.
            In this case object must be destroyed by UniqueObjectPtr's destructor.
     */
    TEST_F(TestSceneBasics, DestroyObject)
    {
        using namespace nau::scene;
        bool componentIsDestructed = false;

        {
            SceneObject::Ptr object = getSceneFactory().createSceneObject<MySceneComponent1>();
            ObjectWeakRef objectRef = *object;
            object->getRootComponent<MySceneComponent1>().setOnDestructor([&]
            {
                componentIsDestructed = true;
            });

            object->destroy();
            ASSERT_FALSE(objectRef);
        }
        ASSERT_TRUE(componentIsDestructed);
    }
    /**
        Test:
            add components to scene object, check component's destructors are called when object are destroyed.
     */
    TEST_F(TestSceneBasics, ObjectAddComponent)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = getSceneFactory().createSceneObject();

        size_t destructorCounter = 0;

        scene::ObjectWeakRef component1Ref = object->addComponent<MyComponent1>();
        ASSERT_EQ(&component1Ref->getParentObject(), object.get());

        scene::ObjectWeakRef component2Ref = object->addComponent<MyComponent2>();
        ASSERT_EQ(&component2Ref->getParentObject(), object.get());

        component1Ref->setOnDestructor([&]
        {
            ++destructorCounter;
        });

        component2Ref->setOnDestructor([&]
        {
            ++destructorCounter;
        });

        // root component + added components
        ASSERT_EQ(object->getDirectComponents().size(), 3);

        object.reset();
        ASSERT_EQ(destructorCounter, 2);
        ASSERT_FALSE(component1Ref);
        ASSERT_FALSE(component2Ref);
    }

    /**
     */
    TEST_F(TestSceneBasics, ObjectRemoveComponent)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = getSceneFactory().createSceneObject();

        scene::ObjectWeakRef objectRef1 = object->addComponent<MyComponent1>();
        ASSERT_TRUE(objectRef1);
        ASSERT_EQ(object->getDirectComponents().size(), 2);

        object->removeComponent(objectRef1);

        ASSERT_EQ(object->getDirectComponents().size(), 1);
        ASSERT_FALSE(objectRef1);
    }

    /**
     */
    TEST_F(TestSceneBasics, GetDirectComponents)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = getSceneFactory().createSceneObject<MySceneComponent1>();
        object->addComponent<MyComponent1>();
        object->addComponent<MyComponent2>();

        auto components = object->getDirectComponents();
        ASSERT_EQ(components.size(), 3);
    }

    /**
     */
    TEST_F(TestSceneBasics, GetAllComponents)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = getSceneFactory().createSceneObject<MySceneComponent1>();
        object->addComponent<MyComponent1>();
        object->addComponent<MyComponent2>();

        {
            SceneObject::Ptr child = getSceneFactory().createSceneObject<MySceneComponent1>();
            child->addComponent<MyComponent1>();
            child->addComponent<MyComponent2>();

            object->attachChild(std::move(child));
        }

        auto components = object->getAllComponents();
        ASSERT_EQ(components.size(), 6);
    }

    /**
     */
    TEST_F(TestSceneBasics, GetComponentsWithType)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = getSceneFactory().createSceneObject<MySceneComponent1>();
        object->addComponent<MySceneComponent1>();
        object->addComponent<MyComponent2>();

        {
            SceneObject::Ptr child = getSceneFactory().createSceneObject<MySceneComponent1>();
            child->addComponent<MyComponent1>();
            child->addComponent<MyComponent2>();

            object->attachChild(std::move(child));
        }

        {
            auto components = object->getAllComponents<MySceneComponent1>();
            ASSERT_EQ(components.size(), 3);
        }

        {
            auto components = object->getDirectComponents<MySceneComponent1>();
            ASSERT_EQ(components.size(), 2);
        }

        {
            auto components = object->getComponents(true, &rtti::getTypeInfo<MyComponent1>());
            ASSERT_EQ(components.size(), 1);
        }
    }

    /**
     */
    TEST_F(TestSceneBasics, FindFirstComponent)
    {
        using namespace nau::scene;

        SceneObject::Ptr object = getSceneFactory().createSceneObject<MySceneComponent1>();
        object->addComponent<MyComponent1>();

        ASSERT_NE(object->findFirstComponent<MyComponent1>(), nullptr);
        ASSERT_NE(object->findFirstComponent<MySceneComponent1>(), nullptr);
        ASSERT_EQ(object->findFirstComponent<MyComponent2>(), nullptr);
    }

}  // namespace nau::test
