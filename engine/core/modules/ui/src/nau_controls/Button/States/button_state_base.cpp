// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "button_state_base.h"

#include "2d/CCSpriteFrame.h"
#include "nau/ui/button.h"
#include <sstream>
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"

namespace nau::ui
{

    ButtonStateBase::ButtonStateBase() = default;

    ButtonStateBase::~ButtonStateBase()
    {
        CC_SAFE_RELEASE(m_stateSpriteFrame);
    }

    bool ButtonStateBase::tryCreateStateSpriteFrame(const std::string& filePath)
    {
        cocos2d::Texture2D* texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(filePath);
        if (!texture)
        {
            NAU_LOG_ERROR("Failed to load texture:{}", filePath);
            return false;
        }
        cocos2d::SpriteFrame* spriteFrame = cocos2d::SpriteFrame::createWithTexture(
            texture,
            cocos2d::Rect(0.0f, 0.0f, texture->getContentSize().width, texture->getContentSize().height));

        if (!spriteFrame)
        {
            NAU_LOG_ERROR("Failed to create sprite frame from texture:{}", filePath);
            return false;
        }

        m_stateSpriteFrame = spriteFrame;
        CC_SAFE_RETAIN(m_stateSpriteFrame);

        return true;
    }

    void ButtonStateBase::setStateColor(const math::Color4& color)
    {
        m_stateColor = color;
    }

    void ButtonStateBase::setupTexture(nau::ui::NauButton* button)
    {
        NAU_ASSERT(button != nullptr, "[setupTexture] button can't be nullptr!");

        nau::ui::Sprite* btnSprite = button->getButtonSprite();

        nau::ui::SpriteFrameHandler stateSpriteFrameContainer;
        stateSpriteFrameContainer.m_spriteFrame = m_stateSpriteFrame;

        if (btnSprite && btnSprite->initWithSpriteFrameContainer(stateSpriteFrameContainer))
        {
            updateSize(button);
        }
    }

    void ButtonStateBase::setupColor(nau::ui::NauButton* button)
    {
        NAU_ASSERT(button != nullptr, "[setupColor] button can't be nullptr!");

        nau::ui::Sprite* btnSprite = button->getButtonSprite();
        if (btnSprite)
        {
            btnSprite->setColor(m_stateColor);
        }
    }

    void ButtonStateBase::setupSize(nau::ui::NauButton* button)
    {
        NAU_ASSERT(button != nullptr, "[setupSize] button can't be nullptr!");

        button->setScale(m_stateScale);
    }

    void ButtonStateBase::updateSize(nau::ui::NauButton* button)
    {
        NAU_ASSERT(button != nullptr, "[updateSize] button can't be nullptr!");

        nau::ui::Sprite* btnSprite = button->getButtonSprite();
        if (btnSprite)
        {
            btnSprite->setContentSize(button->getContentSize());
        }

        button->updateSpriteLocation();
        button->updateTitleLocation();
    }

} // namespace nau::ui