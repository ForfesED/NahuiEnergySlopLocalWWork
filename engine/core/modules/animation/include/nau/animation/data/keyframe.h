// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

namespace nau::animation
{
    /**
     * @brief Base class for a keyframe.
     */
    class NAU_ABSTRACT_TYPE KeyFrame : public virtual IRefCounted
    {
        NAU_CLASS(nau::animation::KeyFrame, rtti::RCPolicy::StrictSingleThread, IRefCounted)
    };

    /**
     * @brief Provides a default implementation for an animation keyframe class.
     * 
     * @tparam TValue Type of the animated parameter.
     */
    template<class TValue>
    class KeyFrameImpl : public KeyFrame
    {
        NAU_CLASS(nau::animation::KeyFrameImpl<TValue>, rtti::RCPolicy::StrictSingleThread, KeyFrame)

    public:

        /**
         * @brief Default constructor.
         */
        KeyFrameImpl() = default;

        /**
         * @brief Initialization constructor.
         * 
         * @param [in] frame Index of the frame in the animation.
         * @param [in] value Keyframe parameter value. See @ref m_value
         */
        KeyFrameImpl(int frame, const TValue& value) 
            : m_frame(frame),
            m_value(value)
        {
        }

        /**
         * @brief Retrieves the keyframe parameter value.
         * 
         * @return Keyframe parameter value.
         * 
         * See m_value.
         */
        const TValue& getValue() const
        {
            return m_value;
        }

        /**
         * @brief Retrieves the index of the keyframe among other (ordinary) frames.
         */
        int getFrame() const
        {
            return m_frame;
        }

    private:

        TValue m_value; ///<Value that is assigned to the animated parameter as the playback reaches the keyframe.

        int m_frame;    ///< Index of the keyframe among other (ordinary) frames.
    };
}  // namespace nau::animation
