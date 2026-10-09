// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/playback/animation_player_keyframe.h"

#include "nau/animation/playback/animation_instance.h"

namespace nau::animation
{
    KeyFrameAnimationPlayer::KeyFrameAnimationPlayer(AnimationInstance& animInstance)
        : m_animInstance(animInstance)
    {
    }

    int KeyFrameAnimationPlayer::getDurationInFrames() const
    {
        if (const Animation* animation = m_animInstance.getAnimation())
        {
            return animation->getDurationInFrames();
        }

        return 0;
    }

    void KeyFrameAnimationPlayer::play()
    {
        m_animInstance.debugIgnoreController(true);
        m_animInstance.setWeight(1.f);
        getAnimState().isStopped = false;
        pause(false);
    }

    void KeyFrameAnimationPlayer::pause(bool pause)
    {
        getAnimState().isPaused = pause;
    }

    void KeyFrameAnimationPlayer::stop()
    {
        getAnimState().time = .0f;
        m_animInstance.m_frame = 0;
        getAnimState().isStopped = true;
    }

    bool KeyFrameAnimationPlayer::isPaused() const
    {
        return getAnimState().isPaused;
    }

    bool KeyFrameAnimationPlayer::isReversed() const
    {
        return getAnimState().isReversed;
    }

    void KeyFrameAnimationPlayer::reverse(bool reverse)
    {
        getAnimState().isReversed = reverse;
    }

    void KeyFrameAnimationPlayer::setPlaybackSpeed(float speed)
    {
        getAnimState().playbackSpeed = speed;
    }

    float KeyFrameAnimationPlayer::getPlaybackSpeed() const
    {
        return getAnimState().playbackSpeed;
    }

    int KeyFrameAnimationPlayer::getPlayingFrame() const
    {
        return m_animInstance.getCurrentFrame();
    }

    void KeyFrameAnimationPlayer::jumpToFirstFrame()
    {
        getAnimState().forcedFrame = 0;
    }

    void KeyFrameAnimationPlayer::jumpToLastFrame()
    {
        if (const Animation* animation = m_animInstance.getAnimation())
        {
            getAnimState().forcedFrame = animation->getLastFrame();
        }
    }

    void KeyFrameAnimationPlayer::jumpToFrame(int frameNum)
    {
        if (const Animation* animation = m_animInstance.getAnimation())
        {
            NAU_CONDITION_LOG(frameNum < 0 || animation->getLastFrame() < frameNum, diag::LogLevel::Warning, "Requested jumpToFrame out of the animation track length ({} of {})", frameNum, animation->getLastFrame());
            getAnimState().forcedFrame = frameNum;
        }
    }

    AnimationState& KeyFrameAnimationPlayer::getAnimState()
    {
        return m_animInstance.m_animationState;
    }

    const AnimationState& KeyFrameAnimationPlayer::getAnimState() const
    {
        return m_animInstance.m_animationState;
    }

    AnimationInstance& KeyFrameAnimationPlayer::getAnimInstance()
    {
        return m_animInstance;
    }

    const AnimationInstance& KeyFrameAnimationPlayer::getAnimInstance() const
    {
        return m_animInstance;
    }

 }  // namespace nau::animation
