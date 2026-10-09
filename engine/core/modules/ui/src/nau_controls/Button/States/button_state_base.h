// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/ui/ui_control.h"
#include "sprite_frame_handler.h"
#include <string>
#include "2d/CCSpriteFrame.h"

namespace nau::ui
{
    class NauButton;
    struct NauButtonData;

    class ButtonStateBase
    {
    public:
        ButtonStateBase();
        virtual ~ButtonStateBase();

        virtual bool initialize(nau::ui::NauButton* button, nau::ui::NauButtonData& data) = 0;
        virtual void enter(nau::ui::NauButton* button) = 0;
        virtual void handleEvent(nau::ui::NauButton* button, nau::ui::EventType eventType) { }
        virtual void update(nau::ui::NauButton* button) { }
        virtual void exit(nau::ui::NauButton* button) = 0;

    protected:
        cocos2d::SpriteFrame* m_stateSpriteFrame{ nullptr };
        cocos2d::Color3B m_stateColor{};
        float m_stateScale{ 0.0f };

        bool tryCreateStateSpriteFrame(const std::string& filePath);
        void setStateColor(const math::Color4& color);

        void setupTexture(nau::ui::NauButton* button);
        void setupColor(nau::ui::NauButton* button);
        void setupSize(nau::ui::NauButton* button);
        void updateSize(nau::ui::NauButton* button);
    };

} // namespace nau::ui