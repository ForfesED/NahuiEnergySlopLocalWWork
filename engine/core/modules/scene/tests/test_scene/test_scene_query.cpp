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
        };

        class MyComponent2 : public scene::SceneComponent
        {
            NAU_OBJECT(nau::test::MyComponent2, scene::SceneComponent)
        };
    }  // namespace

    /**
     */
    class TestSceneQuery : public SceneTestBase
    {
        void initializeApp() override
        {
            SceneTestBase::initializeApp();

            registerClasses<
                MyComponent1,
                MyComponent2>();
        }
    };

    /**
        Test: Converting SceneQuery value to string
     */
    TEST_F(TestSceneQuery, SceneQueryToString)
    {
        scene::SceneQuery query;
        query.category = scene::QueryObjectCategory::Component;
        query.uid = Uid::generate();
        query.setType<MyComponent1>();

        std::string queryStr = toString(query);
        ASSERT_TRUE(!queryStr.empty());
    }

    /**
        Test: Parsing SceneQuery value from string
     */
    TEST_F(TestSceneQuery, SceneQueryParse)
    {
        using namespace nau::strings;

        scene::SceneQuery query;
        query.category = scene::QueryObjectCategory::Component;
        query.uid = Uid::generate();
        query.setType<MyComponent1>();

        std::string queryStr = toString(query);

        scene::SceneQuery query2;
        ASSERT_TRUE(parse(queryStr, query2));
        ASSERT_EQ(query2, query);
    }

    /**
        Test: attempting to parse an empty string should return an error result
     */
    TEST_F(TestSceneQuery, SceneQueryFailParseEmptyString)
    {
        std::string_view emptyStr;
        scene::SceneQuery query;

        ASSERT_FALSE(parse(emptyStr, query));
    }

    /**
        Test: attempting to parse an invalid string should return an error result
     */
    TEST_F(TestSceneQuery, SceneQueryFailParseInvalidString)
    {
        scene::SceneQuery query;

        // unkown category
        ASSERT_FALSE(parse("category=Invalid", query));

        // broken uid value
        ASSERT_FALSE(parse("category=Component,uid=Invalid_Uid", query));

        // unparsable string
        ASSERT_FALSE(parse("invalid_string", query));

        // unkown query parameter
        ASSERT_FALSE(parse("unknown_param=value", query));

        // partially unparsable string
        ASSERT_FALSE(parse("category=Object,$$$", query));
    }

    /**
        Test: query single component by uid
     */
    TEST_F(TestSceneQuery, QuerySingleComponent)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            auto& child1 = scene->getRoot().attachChild(createObject<MyComponent1>());
            auto& child2 = child1.attachChild(createObject<MyComponent2>());

            co_await getSceneManager().activateScene(std::move(scene));

            {
                SceneQuery query{QueryObjectCategory::Component, child1.getRootComponent().getUid()};
                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query);
                ASSERT_ASYNC(componentRef);
                ASSERT_ASYNC(componentRef.get() == child1.getRootComponent().as<NauObject*>());
            }

            {
                // category not explicitly specified:
                SceneQuery query;
                query.uid = child2.getRootComponent().getUid();

                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query);
                ASSERT_ASYNC(componentRef);
                ASSERT_ASYNC(componentRef.get() == child2.getRootComponent().as<NauObject*>());
            }

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test: query single component by uid with a concrete type restriction
     */
    TEST_F(TestSceneQuery, QuerySingleComponentWithType)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            auto& child1 = scene->getRoot().attachChild(createObject<MyComponent1>());
            auto& child2 = child1.attachChild(createObject<MyComponent2>());

            co_await getSceneManager().activateScene(std::move(scene));

            {
                SceneQuery query{QueryObjectCategory::Component, child1.getRootComponent().getUid()};
                // restrict the query with a proper type
                query.setType<MyComponent1>();

                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query);
                ASSERT_ASYNC(componentRef);
                ASSERT_ASYNC(componentRef.get() == child1.getRootComponent().as<NauObject*>());
            }

            {
                SceneQuery query{QueryObjectCategory::Component, child2.getRootComponent().getUid()};
                // restrict the query with an invalid type (child2's root is 'MyComponent2', not 'MyComponent1')
                query.setType<MyComponent1>();

                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query);
                ASSERT_FALSE_ASYNC(componentRef);
            }

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
        Test: query single object by uid
    */
    TEST_F(TestSceneQuery, QuerySingleObject)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            auto& child1 = scene->getRoot().attachChild(createObject<MyComponent1>());
            auto& child2 = child1.attachChild(createObject<MyComponent2>());

            co_await getSceneManager().activateScene(std::move(scene));

            {
                SceneQuery query{QueryObjectCategory::Object, child1.getUid()};
                ObjectWeakRef<> objectRef = getSceneManager().querySingleObject(query);
                ASSERT_ASYNC(objectRef);
                ASSERT_ASYNC(objectRef.get() == child1.as<NauObject*>());
            }

            {
                // category not explicitly specified:
                SceneQuery query;
                query.uid = child2.getUid();
                ObjectWeakRef<> objectRef = getSceneManager().querySingleObject(query);
                ASSERT_ASYNC(objectRef);
                ASSERT_ASYNC(objectRef.get() == child2.as<NauObject*>());
            }

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
         Test: query non existent single object by uid
     */
    TEST_F(TestSceneQuery, QueryNonExistentSingleObject)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            auto& child1 = scene->getRoot().attachChild(createObject<MyComponent1>());
            child1.attachChild(createObject<MyComponent2>());

            co_await getSceneManager().activateScene(std::move(scene));

            {
                SceneQuery query{QueryObjectCategory::Object, Uid::generate()};
                ObjectWeakRef<> objectRef = getSceneManager().querySingleObject(query);
                ASSERT_FALSE_ASYNC(objectRef);
            }

            {
                // category not explicitly specified:
                SceneQuery query;
                query.uid = Uid::generate();
                ObjectWeakRef<> objectRef = getSceneManager().querySingleObject(query);
                ASSERT_FALSE_ASYNC(objectRef);
            }

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
     */
    TEST_F(TestSceneQuery, CreateSingleComponentQuery)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            auto& child1 = scene->getRoot().attachChild(createObject<MyComponent1>());
            auto& child2 = child1.attachChild(createObject<MyComponent2>());

            const SceneQuery query1 = scene::createSingleObjectQuery(child1.getRootComponent());
            const SceneQuery query2 = scene::createSingleObjectQuery(child2.getRootComponent());

            co_await getSceneManager().activateScene(std::move(scene));

            {
                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query1);
                ASSERT_ASYNC(componentRef);
                ASSERT_ASYNC(componentRef.get() == child1.getRootComponent().as<NauObject*>());
            }

            {
                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query2);
                ASSERT_ASYNC(componentRef);
                ASSERT_ASYNC(componentRef.get() == child2.getRootComponent().as<NauObject*>());
            }

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
     */
    TEST_F(TestSceneQuery, CreateSingleObjectQuery)
    {
        using namespace testing;
        using namespace nau::async;
        using namespace nau::scene;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
        {
            IScene::Ptr scene = createEmptyScene();
            auto& child1 = scene->getRoot().attachChild(createObject<MyComponent1>());
            auto& child2 = child1.attachChild(createObject<MyComponent2>());

            const SceneQuery query1 = scene::createSingleObjectQuery(child1);
            const SceneQuery query2 = scene::createSingleObjectQuery(child2);

            co_await getSceneManager().activateScene(std::move(scene));

            {
                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query1);
                ASSERT_ASYNC(componentRef);
                ASSERT_ASYNC(componentRef.get() == child1.as<NauObject*>());
            }

            {
                ObjectWeakRef<> componentRef = getSceneManager().querySingleObject(query2);
                ASSERT_ASYNC(componentRef);
                ASSERT_ASYNC(componentRef.get() == child2.as<NauObject*>());
            }

            co_return AssertionSuccess();
        });

        ASSERT_TRUE(testResult);
    }

    /**
     */
    TEST_F(TestSceneQuery, CreateQueryForInvalidRefReturnsNothing)
    {
        using namespace nau::scene;

        const SceneQuery query = scene::createSingleObjectQuery(ObjectWeakRef<>{});
        ASSERT_EQ(query.uid, NullUid);
        ASSERT_FALSE(query.hasType());
        ASSERT_FALSE(query.category.has_value());
    }

}  // namespace nau::test