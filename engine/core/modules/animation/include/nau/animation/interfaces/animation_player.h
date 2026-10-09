// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

namespace nau::animation
{
    /**
     * @brief Provides an interface for controlling a single animation playback.
     */
    class IAnimationPlayer : public virtual IRefCounted
    {
        NAU_INTERFACE(nau::animation::IAnimationPlayer, IRefCounted)

        using Ptr = nau::Ptr<IAnimationPlayer>;

        /**
         * @brief Retrieves the animation duration in frames.
         * 
         * @return Number of frames composing the animation
         */
        virtual int getDurationInFrames() const = 0;

        /**
         * @brief Continues the animation playback.
         */
        virtual void play() = 0;

        /**
         * Pauses or continues animation playback.
         * 
         * @param [in] pause Indicates whether the playback should be paused or not.
         */
        virtual void pause(bool pause) = 0;

        /**
         * @brief Stops the playback and resets the animation to the first frame.
         */
        virtual void stop() = 0;

        /**
         * @brief Checks whether the playback is currently paused.
         * 
         * @return `true` if the animation is on pause, `false` otherwsise.
         */
        virtual bool isPaused() const = 0;

        /**
         * @brief Retrieves the playback direction.
         * 
         * @return `true` if the playback direction is reversed, `false` if it is forward.
         */
        virtual bool isReversed() const = 0;

        /**
         * @brief Switches the direction of the playback.
         * 
         * @param [in] Indicates whether the playback direction should be forward or reversed.
         */
        virtual void reverse(bool reverse) = 0;

        /**
         * @brief Changes the animation playback speed value.
         * 
         * @param [in] Value to assign.
         */
        virtual void setPlaybackSpeed(float speed) = 0;

        /**
         * @brief Retrieves the animation playback speed.
         * 
         * @return Animation playback speed.
         */
        virtual float getPlaybackSpeed() const = 0;

        /**
         * @brief Retrieves the index of the animation frame that the player is currently playing.
         * 
         * @return Animation played frame.
         */
        virtual int getPlayingFrame() const = 0;

        /**
         * @brief Changes the currently played frame to the first one.
         */
        virtual void jumpToFirstFrame() = 0;

        /**
         * @brief Changes the currently played frame to the last one. 
         * 
         * This effectively finishes the playback.
         */
        virtual void jumpToLastFrame() = 0;

        /**
         * @brief Changes the currently played frame to the specified index.
         * 
         * @param [in] Index of the frame the playback will continue with.
         */
        virtual void jumpToFrame(int frameNum) = 0;
    };
} // namespace nau::animation