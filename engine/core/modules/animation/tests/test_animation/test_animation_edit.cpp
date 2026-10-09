// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/components/animation_component.h"
#include "nau/animation/playback/animation_transforms.h"
#include "nau/scene/scene_object.h"
#include "scene_test_base.h"

namespace nau::test
{
    class TestAnimation : public SceneTestBase
    {
    private:
        void initializeApp() override
        {
        }
    };

    TEST_F(TestAnimation, EditKeyFramesApi)
    {
        using namespace animation;
        using namespace math;
        using namespace scene;

        IScene::Ptr scene = createEmptyScene();
        
        auto& animatedObject = scene->getRoot().attachChild(createObject());
        auto& animComp = animatedObject.addComponent<AnimationComponent>();

        auto animation = rtti::createInstance<TransformAnimation>();

        if (auto animationEditor = animation->createEditor())
        {
            animationEditor.addKeyFrame(300, Transform::identity());
            animationEditor.addKeyFrame(0, Transform(quat::identity(), vec3(-10.f, .0f, 10.0f), vec3(1.f, 1.f, 1.f)));
        }

        EXPECT_TRUE(animation->getNumKeyFrames() == 2);
        EXPECT_TRUE(animation->getLastFrame() == 300);
        EXPECT_TRUE(animation->getKeyFrameAt(1) && animation->getKeyFrameAt(1)->getFrame() == 300);

        auto animInstance = rtti::createInstance<AnimationInstance>("test", animation);
        animInstance->setPlayMode(PlayMode::Looping);
        animComp.addAnimation(animInstance);

        if (auto animationEditor = animation->createEditor())
        {
            animationEditor.addKeyFrame(320, Transform(quat::identity(), vec3(-10.f, .0f, 10.0f), vec3(1.f, 1.f, 1.f)));
        }

        EXPECT_TRUE(animation->getNumKeyFrames() == 3);
        EXPECT_TRUE(animation->getLastFrame() == 320);
        EXPECT_TRUE(animation->getKeyFrameAt(1) && animation->getKeyFrameAt(1)->getFrame() == 300);

        if (auto animationEditor = animation->createEditor())
        {
            animationEditor.deleteKeyFrame(300);
        }

        EXPECT_TRUE(animation->getNumKeyFrames() == 2);
        EXPECT_TRUE(animation->getLastFrame() == 320);
        EXPECT_TRUE(animation->getKeyFrameAt(1) && animation->getKeyFrameAt(1)->getFrame() == 320);

    }

    TEST_F(TestAnimation, EditEventsApi)
    {
        using namespace animation;
        using namespace math;
        using namespace scene;

        IScene::Ptr scene = createEmptyScene();

        auto& animatedObject = scene->getRoot().attachChild(createObject());
        auto& animComp = animatedObject.addComponent<AnimationComponent>();

        auto animation = rtti::createInstance<TransformAnimation>();

        if (auto animationEditor = animation->createEditor())
        {
            animationEditor.addFrameEvent(100, FrameEvent("my one-time event"));
            animationEditor.addFrameEvent(1, FrameEvent("my one-time event"));
            animationEditor.addFrameEvent(100, FrameEvent("my one-time event"));
            animationEditor.addFrameEvent(50, FrameEvent("my one-time event"));
        }

        EXPECT_TRUE(animation->asInplaceEditor().getFrameDataCount() == 3);
        EXPECT_TRUE(animation->asInplaceEditor().getFrameDataAtIndex(2).frame == 100);

        EXPECT_TRUE(animation->asInplaceEditor().getEventCount(100) == 2);
        EXPECT_TRUE(animation->asInplaceEditor().getFrameDataAtIndex(2).events.size() == 2);
    }
}  // namespace nau::test
