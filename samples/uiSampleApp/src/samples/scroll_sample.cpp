// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "scroll_sample.h"
#include "nau/ui/symbol_factory.h"
#include <EASTL/unique_ptr.h>

namespace nau::sample
{
    nau::ui::NauScroll* createMainScroll(const cocos2d::Vec2& position)
    {
        auto scroll = nau::ui::NauScroll::create(nau::ui::NauScroll::ScrollType::vertical, {500, 500});
        scroll->setPosition(position);
        
        auto sprite = nau::ui::Sprite::create("/res/Images/nau_button_pressed.png");
        scroll->addScrollBarSprite(sprite);

        return scroll;
    }

    nau::ui::NauButton* createSimpleButton(const cocos2d::Vec2& position)
    {
        using namespace nau::math;

        nau::ui::NauButtonData data;

        data.defaultImageFileName = "/res/Images/nau_button_active.png";
        data.hoveredImageFileName = "/res/Images/nau_button_hover.png";
        data.clickedImageFileName = "/res/Images/nau_button_pressed.png";
        data.disableImageFileName = "/res/Images/nau_button_disabled.png";

        data.clickedScale = 1.5f;

        nau::ui::NauButton* button = nau::ui::NauButton::create(data);
        button->setPosition(position);
        button->setContentSize({36.0f, 36.0f});

        auto label = nau::ui::NauLabel::create(
            "Btn",
            "/res/fonts/bitmapFontTest4.fnt",
            nau::ui::HorizontalAlignment::center,
            nau::ui::VerticalAlignment::center,
            nau::ui::NauLabel::Overflow::none,
            nau::ui::NauLabel::Wrapping::word);

        label->setContentSize(button->getContentSize());
        label->setScale(0.5f);
        label->updateLabel();

        button->setTitleLabel(label);

        return button;
    }

    async::Task<> scrollScene(nau::ui::Canvas* sampleUiScene)
    {
        auto engineScene = getServiceProvider().get<scene::ISceneFactory>().createEmptyScene();
        engineScene->setName("UI service scene");
        nau::getServiceProvider().get<nau::ui::UiManager>().setEngineScene(engineScene.getRef());

        auto& window = nau::getServiceProvider().get<nau::IWindowManager>().getActiveWindow();
        {
            window.setVisible(true);
            const auto [x, y] = window.getClientSize();
            nau::input::setScreenResolution(x, y);
            nau::getServiceProvider().get<nau::ui::UiManager>().setScreenSize(x, y);
            nau::getServiceProvider().get<nau::ui::UiManager>().setReferenceResolution(x, y);
            nau::getServiceProvider().get<nau::ui::UiManager>().configureResourcePath();

            sampleUiScene->setReferenceSize({ float(x), float(y) });
        }

        sampleUiScene->setRescalePolicy(nau::ui::RescalePolicy::NoRescale);


        auto scroll = createMainScroll(sampleUiScene->getReferenceSize() * 0.5f);
        sampleUiScene->addChild(scroll, "MainScroll");

        std::string labelTextHello = "<color=0xFF00FF00>Hello</color>";
        auto labelHello = nau::ui::NauLabel::create(labelTextHello.c_str(), "/res/fonts/bitmapFontTest4.fnt",
        nau::ui::HorizontalAlignment::center,
        nau::ui::VerticalAlignment::center,
        nau::ui::NauLabel::Overflow::none,
        nau::ui::NauLabel::Wrapping::character);

        labelHello->setContentSize({scroll->getContentSize().getX(), 50.0f});
        labelHello->updateLabel();
        scroll->addChildWithAlignment(labelHello);


        std::string labelTextName = "<color=0xFF0000FF>My name is</color>";
        auto labelName = nau::ui::NauLabel::create(labelTextName.c_str(), "/res/fonts/bitmapFontTest2.fnt",
        nau::ui::HorizontalAlignment::left,
        nau::ui::VerticalAlignment::center,
        nau::ui::NauLabel::Overflow::none,
        nau::ui::NauLabel::Wrapping::character);

        labelName->setContentSize({scroll->getContentSize().getX(), 75.0f});
        labelName->updateLabel();
        scroll->addChildWithAlignment(labelName);

        std::string labelTextScroll = "<color=0xFFFF0000>Scroll</color>";
        auto labelScroll = nau::ui::NauLabel::create(labelTextScroll.c_str(), "/res/fonts/bitmapFontTest3.fnt",
        nau::ui::HorizontalAlignment::right,
        nau::ui::VerticalAlignment::center,
        nau::ui::NauLabel::Overflow::none,
        nau::ui::NauLabel::Wrapping::character);

        labelScroll->setContentSize({scroll->getContentSize().getX(), 50.0f});
        labelScroll->updateLabel();
        scroll->addChildWithAlignment(labelScroll);

        eastl::unique_ptr<nau::ui::SymbolFactory> symbolFactory = eastl::make_unique<nau::ui::SymbolFactory>();
        symbolFactory->registerProvider("res/fonts/bitmapFontTest4.fnt");
        symbolFactory->registerProvider("res/fonts/bitmapFontTest3.fnt");
        symbolFactory->registerProvider("res/fonts/bitmapFontTest2.fnt");

        auto labelWithSymbolFactory = nau::ui::NauLabel::create(eastl::move(symbolFactory));
        labelWithSymbolFactory->setContentSize({scroll->getContentSize().getX(), 75.0f});
        labelWithSymbolFactory->setHorizontalAlignment(nau::ui::HorizontalAlignment::center);
        labelWithSymbolFactory->setVerticalAlignment(nau::ui::VerticalAlignment::center);
        labelWithSymbolFactory->setOverflowType(nau::ui::NauLabel::Overflow::none);
        labelWithSymbolFactory->setWrapping(nau::ui::NauLabel::Wrapping::word);
        labelWithSymbolFactory->setText("<font=bitmapFontTest2>font2</font> <font=bitmapFontTest3>font3</font> <font=bitmapFontTest4>font4</font>");
        scroll->addChildWithAlignment(labelWithSymbolFactory);

        const int contentItemsCount = 10;
        for (int i = 0; i < contentItemsCount; i++) 
        {
            std::string fillerText = "<color=0xFFF6768E>^^^</color>";
            auto fillerLabel = nau::ui::NauLabel::create(fillerText.c_str(), "/res/fonts/bitmapFontTest4.fnt",
            nau::ui::HorizontalAlignment::center,
            nau::ui::VerticalAlignment::center,
            nau::ui::NauLabel::Overflow::none,
            nau::ui::NauLabel::Wrapping::character);

            fillerLabel->setContentSize(cocos2d::Size(scroll->getContentSize().getX(), 25.0f));
            fillerLabel->updateLabel();
            scroll->addChildWithAlignment(fillerLabel);
        }

        nau::ui::Node* buttonAllignContainer = nau::ui::Node::create();
        buttonAllignContainer->setContentSize({scroll->getContentSize().getX(), 50.0f});
        auto button = createSimpleButton(buttonAllignContainer->getContentSize() * 0.5f);
        button->getTitleLabel()->setText("LOL");
        buttonAllignContainer->addChild(button);
        scroll->addChildWithAlignment(buttonAllignContainer);

        


        for (int i = 0; i < contentItemsCount; i++) 
        {
            std::string fillerText = "<color=0xFFF6768E>^^^</color>";
            auto fillerLabel = nau::ui::NauLabel::create(fillerText.c_str(), "/res/fonts/bitmapFontTest4.fnt",
            nau::ui::HorizontalAlignment::center,
            nau::ui::VerticalAlignment::center,
            nau::ui::NauLabel::Overflow::none,
            nau::ui::NauLabel::Wrapping::character);

            fillerLabel->setContentSize(cocos2d::Size(scroll->getContentSize().getX(), 25.0f));
            fillerLabel->updateLabel();
            scroll->addChildWithAlignment(fillerLabel);
        }

        nau::ui::Node* spriteAllignContainer = nau::ui::Node::create();
        spriteAllignContainer->setContentSize({scroll->getContentSize().getX(), 100.0f});
        nau::ui::Sprite* nestedSprite = nau::ui::Sprite::create("/res/Images/YellowSquare.png");

        nau::ui::Sprite* nestedNestedSprite = nau::ui::Sprite::create("/res/Images/nau_button_active.png");
        nestedSprite->addChild(nestedNestedSprite);
        nestedNestedSprite->setPosition(nestedSprite->getContentSize() * 0.5f);

        spriteAllignContainer->addChild(nestedSprite);
        nestedSprite->setPosition(spriteAllignContainer->getContentSize() * 0.5f);
        scroll->addChildWithAlignment(spriteAllignContainer);

        std::string labelWithImageText = "<image src=res/Images/nau_button_hover.png><color=0xFFFF0000>Scroll-end</color><image src=res/Images/nau_button_hover.png>";
        auto labelWithImage = nau::ui::NauLabel::create(labelWithImageText.c_str(), "/res/fonts/bitmapFontTest3.fnt",
        nau::ui::HorizontalAlignment::center,
        nau::ui::VerticalAlignment::center,
        nau::ui::NauLabel::Overflow::none,
        nau::ui::NauLabel::Wrapping::character);

        labelWithImage->setContentSize(cocos2d::Size(scroll->getContentSize().getX(), 50.0f));
        labelWithImage->updateLabel();
        scroll->addChildWithAlignment(labelWithImage);

        scroll->moveTo(labelHello);

        co_await getServiceProvider().get<scene::ISceneManager>().activateScene(std::move(engineScene));
    }
}