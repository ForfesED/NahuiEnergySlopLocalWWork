// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/assets/asset_container.h"
#include "nau/assets/asset_ref.h"
#include "nau/io/memory_stream.h"
#include "nau/io/virtual_file_system.h"
#include "nau/scene/scene_factory.h"
#include "nau/service/service_provider.h"
#include "scene_test_base.h"
#include "scene_test_components.h"

namespace nau::test
{
    namespace
    {
        struct MyTestStruct
        {
            float x = 11.f;
            float y = 22.f;
            std::vector<std::string> tags;

            NAU_CLASS_FIELDS(
                CLASS_FIELD(x),
                CLASS_FIELD(y),
                CLASS_FIELD(tags))
        };

        class ComponentWithData1 : public scene::Component
        {
            NAU_OBJECT(nau::test::ComponentWithData1, scene::Component)
            NAU_DECLARE_DYNAMIC_OBJECT

            NAU_CLASS_FIELDS(
                CLASS_NAMED_FIELD(m_strField, "my_str"),
                CLASS_NAMED_FIELD(m_intField, "my_int"),
                CLASS_NAMED_FIELD(m_structField, "my_struct"))

        public:
            std::string m_strField = "strField";
            unsigned m_intField = 75;
            MyTestStruct m_structField;
        };

        NAU_IMPLEMENT_DYNAMIC_OBJECT(ComponentWithData1)
    }  // namespace

    class TestSceneAsset : public SceneTestBase
    {
    protected:

        static scene::IScene::Ptr cloneScene(scene::IScene& srcScene)
        {
            SceneAsset::Ptr srcSceneAsset = scene::wrapSceneAsAsset(srcScene);
            return getSceneFactory().createSceneFromAsset(*srcSceneAsset);
        }

        static scene::IScene::Ptr makeSceneWithHierarchy()
        {
            auto scene = createEmptyScene();
            scene->getRoot().setName("root");

            auto& child1 = scene->getRoot().attachChild(createObject<scene_test::MyDefaultSceneComponent>("child_1"));
            auto& child2 = scene->getRoot().attachChild(createObject<scene_test::MyDefaultSceneComponent>("child_2"));

            auto& child1_1 = child1.attachChild(createObject("child_1_1"));
            {
                auto& component11 = child1_1.addComponent<ComponentWithData1>();
                component11.m_intField = 11;
                component11.m_strField = "Component11";
                component11.m_structField.tags.push_back("tag111");
                component11.m_structField.tags.push_back("tag112");
            }

            {
                auto& component12 = child1_1.addComponent<ComponentWithData1>();
                component12.m_intField = 12;
                component12.m_strField = "Component12";
                component12.m_structField.tags.push_back("tag121");
                component12.m_structField.tags.push_back("tag122");
                component12.m_structField.tags.push_back("tag123");
            }

            auto& child2_1 = child2.attachChild(createObject("child_2_1"));
            {
                auto& component21 = child2_1.addComponent<ComponentWithData1>();
                component21.m_intField = 21;
                component21.m_strField = "Component21";
                component21.m_structField.tags.push_back("tag211");
                component21.m_structField.tags.push_back("tag212");
                component21.m_structField.tags.push_back("tag213");
            }
            {
                auto& component22 = child2_1.addComponent<ComponentWithData1>();
                component22.m_intField = 22;
                component22.m_strField = "Component22";
                component22.m_structField.tags.push_back("tag221");
                component22.m_structField.tags.push_back("tag222");
                component22.m_structField.tags.push_back("tag223");
            }

            return scene;
        }

    private:
        void initializeApp() override
        {
            SceneTestBase::initializeApp();
            getServiceProvider().addClass<ComponentWithData1>();
        }
    };

    /**
     */
    TEST_F(TestSceneAsset, WrapEmptySceneAsAsset)
    {
        scene::IScene::Ptr scene = createEmptyScene();
        SceneAsset::Ptr sceneAsset = scene::wrapSceneAsAsset(*scene);
        ASSERT_TRUE(sceneAsset);
        ASSERT_EQ(sceneAsset->getSceneInfo().assetKind, SceneAssetKind::Scene);

        scene::IScene::Ptr scene2 = getSceneFactory().createSceneFromAsset(*sceneAsset);
        ASSERT_TRUE(scene2);
        ASSERT_TRUE(scenesEqualSimple(*scene, *scene2));
    }

    /**
     */
    TEST_F(TestSceneAsset, CheckSceneRoot)
    {
        scene::IScene::Ptr scene = createEmptyScene();
        auto& component1 = scene->getRoot().addComponent<ComponentWithData1>();
        component1.m_structField.tags.push_back("tag1");
        component1.m_structField.tags.push_back("tag2");

        auto& component2 = scene->getRoot().addComponent<ComponentWithData1>();
        component1.m_structField.tags.push_back("tag3");
        component2.m_structField.tags.push_back("tag4");

        scene::IScene::Ptr scene2 = cloneScene(*scene);
        ASSERT_TRUE(scenesEqualSimple(*scene, *scene2));
    }

    /**
     */
    TEST_F(TestSceneAsset, WrapEmptySceneObjectAsAsset)
    {
        scene::SceneObject::Ptr object = createObject("TestObject");
        SceneAsset::Ptr prefabAsset = scene::wrapSceneObjectAsAsset(*object);
        ASSERT_TRUE(prefabAsset);
        ASSERT_EQ(prefabAsset->getSceneInfo().assetKind, SceneAssetKind::Prefab);

        scene::SceneObject::Ptr object2 = getSceneFactoryInternal().createSceneObjectFromAssetWithOptions(*prefabAsset, {});
        ASSERT_TRUE(object2);
        ASSERT_TRUE(sceneObjectsEqualSimple(*object, *object2));
    }

    /**
     */
    TEST_F(TestSceneAsset, SceneClone)
    {
        auto scene = makeSceneWithHierarchy();
        auto sceneClone = cloneScene(*scene);
        ASSERT_TRUE(scenesEqualSimple(*scene, *sceneClone));
    }

    /**
     */
    TEST_F(TestSceneAsset, DumpSceneToStreamAndCreateCopy)
    {
        using namespace nau::async;
        using namespace nau::io;
        using namespace nau::scene;

        auto scene = makeSceneWithHierarchy();

        IMemoryStream::Ptr stream = createMemoryStream(AccessMode::Read | AccessMode::Write);

        {
            SceneAsset::Ptr sceneAsset = scene::wrapSceneAsAsset(*scene);
            ASSERT_EQ(sceneAsset->getSceneInfo().assetKind, SceneAssetKind::Scene);
            IAssetContainerBuilder* const assetBuilder = findSceneBuilder(sceneAsset);
            ASSERT_TRUE(assetBuilder);
            const Result<> writeResult = assetBuilder->writeAssetToStream(stream, sceneAsset);
            ASSERT_TRUE(writeResult);
        }
        ASSERT_TRUE(stream->getPosition() > 0);

        IAssetContainerLoader* const assetLoader = findSceneLoader();
        ASSERT_TRUE(assetLoader);

        Task<IAssetContainer::Ptr> assetContainerTask = async::run([](IAssetContainerLoader* loader, IStreamReader::Ptr stream) -> Task<IAssetContainer::Ptr>
        {
            AssetContentInfo assetInfo{
                .kind = "nscene"};

            stream->setPosition(OffsetOrigin::Begin, 0);
            auto container = co_await loader->loadFromStream(stream, assetInfo);
            co_return container;
        }, Executor::getDefault(), assetLoader, stream);

        async::wait(assetContainerTask);

        auto assetContainer = *assetContainerTask;

        nau::Ptr<SceneAsset> sceneAsset = assetContainer->getAsset();
        ASSERT_TRUE(sceneAsset);
        ASSERT_EQ(sceneAsset->getSceneInfo().assetKind, SceneAssetKind::Scene);

        IScene::Ptr sceneCopy = getSceneFactory().createSceneFromAsset(*sceneAsset);
        ASSERT_TRUE(scenesEqualSimple(*scene, *sceneCopy));
    }

    /**
     */
    TEST_F(TestSceneAsset, DumpObjectToStreamAndCreateCopy)
    {
        using namespace nau::async;
        using namespace nau::io;
        using namespace nau::scene;

        auto scene = makeSceneWithHierarchy();

        std::string ssa = sceneObjectToString(scene->getRoot());

        IMemoryStream::Ptr stream = createMemoryStream(AccessMode::Read | AccessMode::Write);

        {
            SceneAsset::Ptr prefabAsset = scene::wrapSceneObjectAsAsset(scene->getRoot());
            ASSERT_EQ(prefabAsset->getSceneInfo().assetKind, SceneAssetKind::Prefab);

            IAssetContainerBuilder* const assetBuilder = findSceneBuilder(prefabAsset);
            ASSERT_TRUE(assetBuilder);
            const Result<> writeResult = assetBuilder->writeAssetToStream(stream, prefabAsset);
            ASSERT_TRUE(writeResult);
        }
        ASSERT_TRUE(stream->getPosition() > 0);

        IAssetContainerLoader* const assetLoader = findSceneLoader();
        ASSERT_TRUE(assetLoader);

        Task<IAssetContainer::Ptr> assetContainerTask = async::run([](IAssetContainerLoader* loader, IStreamReader::Ptr stream) -> Task<IAssetContainer::Ptr>
        {
            AssetContentInfo assetInfo{
                .kind = "nprefab"};

            stream->setPosition(OffsetOrigin::Begin, 0);
            auto container = co_await loader->loadFromStream(stream, assetInfo);
            co_return container;
        }, Executor::getDefault(), assetLoader, stream);

        async::wait(assetContainerTask);

        auto assetContainer = *assetContainerTask;

        nau::Ptr<SceneAsset> prefabAsset = assetContainer->getAsset();
        ASSERT_TRUE(prefabAsset);
        ASSERT_EQ(prefabAsset->getSceneInfo().assetKind, SceneAssetKind::Prefab);

        // using createSceneObjectFromAssetWithOptions to force to do not renew uids
        SceneObject::Ptr objectCopy = getSceneFactoryInternal().createSceneObjectFromAssetWithOptions(*prefabAsset, {});
        std::string ss1 = sceneObjectToString(*objectCopy);
        

        ASSERT_TRUE(sceneObjectsEqualSimple(scene->getRoot(), *objectCopy));
    }

}  // namespace nau::test
