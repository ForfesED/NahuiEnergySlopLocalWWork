// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/animation/interfaces/animation_player.h"

namespace nau::animation
{
    struct AnimationState;
    class AnimationInstance;

    class KeyFrameAnimationPlayer : public IAnimationPlayer
    {
        NAU_CLASS(KeyFrameAnimationPlayer, rtti::RCPolicy::StrictSingleThread, IAnimationPlayer)

    public:
        KeyFrameAnimationPlayer(AnimationInstance&);

        virtual int getDurationInFrames() const override;

        virtual void play() override;
        virtual void pause(bool pause) override;
        virtual void stop() override;
        virtual bool isPaused() const override;
        virtual bool isReversed() const override;
        virtual void reverse(bool reverse) override;

        virtual void setPlaybackSpeed(float speed) override;
        virtual float getPlaybackSpeed() const override;

        virtual int getPlayingFrame() const override;
        virtual void jumpToFirstFrame() override;
        virtual void jumpToLastFrame() override;
        virtual void jumpToFrame(int frameNum) override;

    protected:
        AnimationState& getAnimState();
        const AnimationState& getAnimState() const;
        AnimationInstance& getAnimInstance();
        const AnimationInstance& getAnimInstance() const;

    private:
        AnimationInstance& m_animInstance;
    };

}  // namespace nau::animation
