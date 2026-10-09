// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/components/animation_component.h"
#include "nau/animation/data/events.h"
#include "nau/animation/playback/animation_transforms.h"
#include "nau/messaging/messaging.h"
#include "nau/scene/scene_object.h"
#include "scene_test_base.h"

namespace nau::test
{
    class TestAnimation : public SceneTestBase
    {
    public:
        async::Task<> skipAnimFrames(animation::AnimationController& controller, int frameCount)
        {
            const int startFrame = controller.getCurrentFrame();
            const int targetFrame = startFrame + frameCount;

            while (targetFrame > controller.getCurrentFrame())
            {
                co_await skipFrames(1);
            }
        }
    private:
        void initializeApp() override
        {
        }
    };

    TEST_F(TestAnimation, PlaybackApi)
    {
        using namespace nau::animation;
        using namespace nau::async;
        using namespace nau::math;
        using namespace nau::scene;
        using namespace testing;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
            {
                IScene::Ptr scene = createEmptyScene();

                auto& animatedObject = scene->getRoot().attachChild(createObject());
                auto& animComp = animatedObject.addComponent<AnimationComponent>();

                auto animation = rtti::createInstance<TransformAnimation>();

                if (auto animationEditor = animation->createEditor())
                {
                    animationEditor.addKeyFrame(0, Transform::identity());
                    animationEditor.addKeyFrame(400, Transform(quat::identity(), vec3(-10.f, .0f, 10.0f), vec3(1.f, 1.f, 1.f)));
                }

                auto animInstance = rtti::createInstance<AnimationInstance>("test-anim", animation);
                animInstance->setPlayMode(PlayMode::Looping);
                animComp.addAnimation(animInstance);

                ObjectWeakRef sceneRef = co_await getSceneManager().activateScene(std::move(scene));

                co_await skipFrames(1);

                const int activationFrameNum = animInstance->getCurrentFrame();
                co_await skipAnimFrames(*animComp.getController(), 10);
                const int frameNum0 = animInstance->getCurrentFrame();
                ASSERT_ASYNC(frameNum0 > activationFrameNum);

                animInstance->getPlayer()->pause(true);
                co_await skipAnimFrames(*animComp.getController(), 10);
                const int frameNum1 = animInstance->getCurrentFrame();
                ASSERT_ASYNC(frameNum1 == frameNum0);

                animInstance->getPlayer()->stop();
                co_await skipAnimFrames(*animComp.getController(), 5);
                const int frameNum2 = animInstance->getCurrentFrame();
                ASSERT_ASYNC(frameNum2 == 0);

                animInstance->getPlayer()->play();
                co_await skipAnimFrames(*animComp.getController(), 10);
                const int frameNum3 = animInstance->getCurrentFrame();
                ASSERT_ASYNC(frameNum3 > frameNum2);

                animInstance->getPlayer()->jumpToFirstFrame();
                co_await skipAnimFrames(*animComp.getController(), 1);
                const int frameNum4 = animInstance->getCurrentFrame();
                ASSERT_ASYNC(frameNum4 == 0);

                animInstance->getPlayer()->jumpToFrame(250);
                co_await skipAnimFrames(*animComp.getController(), 1);
                const int frameNum5 = animInstance->getCurrentFrame();
                ASSERT_ASYNC(frameNum5 == 250);

                animInstance->getPlayer()->jumpToLastFrame();
                co_await skipAnimFrames(*animComp.getController(), 1);
                const int frameNum6 = animInstance->getCurrentFrame();
                ASSERT_ASYNC(frameNum6 == 400);

                co_return AssertionSuccess();
            });

        ASSERT_TRUE(testResult);
    }

    TEST_F(TestAnimation, PlaybackEvents)
    {
        using namespace nau::animation;
        using namespace nau::async;
        using namespace nau::math;
        using namespace nau::scene;
        using namespace testing;

        const AssertionResult testResult = runTestApp([&]() -> Task<AssertionResult>
            {
                IScene::Ptr scene = createEmptyScene();

                auto& animatedObject = scene->getRoot().attachChild(createObject());
                auto& animComp = animatedObject.addComponent<AnimationComponent>();

                auto animation = rtti::createInstance<TransformAnimation>();

                constexpr int ANIM_LENGTH_FRAMES = 10;

                if (auto animationEditor = animation->createEditor())
                {
                    animationEditor.addKeyFrame(0, Transform::identity());
                    animationEditor.addKeyFrame(ANIM_LENGTH_FRAMES, Transform(quat::identity(), vec3(-10.f, .0f, 10.0f), vec3(1.f, 1.f, 1.f)));
                }

                auto animInstance = rtti::createInstance<AnimationInstance>("test-anim", animation);
                animInstance->setPlayMode(PlayMode::Looping);
                animComp.addAnimation(animInstance);

                ObjectWeakRef sceneRef = co_await getSceneManager().activateScene(std::move(scene));

                int trackStartedCounter = 0;
                int trackFinishedCounter = 0;

                auto subscription = events::AnimTrackPlaybackEvent.subscribe(
                    animatedObject.getMessageSource(),
                    [&trackStartedCounter, &trackFinishedCounter](const events::FrameEventData& message)
                    {
                        if (message.eventId == events::ANIMATION_EVENT_TRACK_STARTED)
                        {
                            ++trackStartedCounter;
                        }
                        else if (message.eventId == events::ANIMATION_EVENT_TRACK_FINISHED)
                        {
                            ++trackFinishedCounter;
                        }
                    });


                co_await skipFrames(3 * ANIM_LENGTH_FRAMES);

                ASSERT_ASYNC(trackStartedCounter != 0);
                ASSERT_ASYNC(trackFinishedCounter != 0);

                co_return AssertionSuccess();
            });

        ASSERT_TRUE(testResult);
    }

}  // namespace nau::test
