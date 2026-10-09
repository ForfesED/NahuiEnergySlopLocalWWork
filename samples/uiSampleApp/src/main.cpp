// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/components/animation_component.h"
#include "nau/animation/playback/animation_transforms.h"
#include "nau/animation/playback/animation_graphic_props.h"
#include "nau/app/run_application.h"
#include "nau/math/math.h"
#include "nau/math/transform.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/samples/sample_app_delegate.h"
#include "nau/scene/scene.h"
#include "nau/scene/scene_factory.h"
#include "nau/scene/scene_manager.h"
#include "nau/service/service_provider.h"
#include "nau/ui.h"
#include "nau/ui/elements/draw_node.h"
#include "nau/ui/label.h"
#include "nau/ui/button.h"
#include "nau/ui/slider.h"
#include "nau/ui/scroll.h"
#include "nau/ui/button_data.h"
#include "nau/ui/elements/canvas.h"
#include "nau/ui/elements/node.h"
#include "nau/ui/elements/sprite.h"
#include "nau/ui/assets/ui_asset.h"
#include "nau/ui/data/ui_canvas_builder.h"
#include "nau/ui/elements/draw_node.h"
#include "nau/input.h"
#include "renderer/CCTextureCache.h"
#include "platform/CCFileUtils.h"
#include "samples/scroll_sample.h"

namespace nau::sample
{
    scene::ISceneFactory& getSceneFactory()
    {
        return getServiceProvider().get<scene::ISceneFactory>();
    }

    scene::ISceneManager& getSceneManager()
    {
        return getServiceProvider().get<scene::ISceneManager>();
    }

   nau::ui::NauScroll* _scr;

    template<class TAnimation>
    auto animate(nau::animation::IAnimationTarget* target, animation::AnimationComponent& animComp)
    {
        using namespace animation;
        using namespace math;

        auto animation = rtti::createInstance<TAnimation>();

        auto animInstance = rtti::createInstance<AnimationInstance>("test-anim", animation);
        animInstance->setPlayMode(PlayMode::PingPong);
        animComp.addAnimation(animInstance);

        animComp.addCustomAnimationTarget(target, animInstance->getPlayer());

        return animation->createEditor();
    }

    nau::ui::NauLabel* createLabel(const cocos2d::Vec2 position)
    {
        using namespace nau::math;

        auto label = nau::ui::NauLabel::create("Hello world!", "/res/fonts/bitmapFontTest2.fnt");
        label->setContentSize(cocos2d::Size(100.0f, 100.0f));
        label->setPosition(position);

        if (auto animationEditor = animate<animation::TransformAnimation>(label->getAnimator(), *label->getAnimationComponent()))
        {
            animationEditor.addKeyFrame(0, Transform::identity());
            animationEditor.addKeyFrame(400, Transform(quat::rotationY(1.57f), vec3(500.f, 500.0f, .0f), vec3(1.f, 1.f, 1.f)));
        }

        if (auto animationEditor = animate<animation::OpacityAnimation>(label->getAnimator(), *label->getAnimationComponent()))
        {
            animationEditor.addKeyFrame(0, 1.f);
            animationEditor.addKeyFrame(400, .0f);
        }

        return label;
    }

    nau::ui::NauButton* createButton(const cocos2d::Vec2& position)
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

        if (auto animationEditor = animate<animation::ColorAnimation>(button->getAnimator(), *button->getAnimationComponent()))
        {
            animationEditor.addKeyFrame(0, Color3(1.f, .0f, .0f));
            animationEditor.addKeyFrame(100, Color3(.0f, 1.f, .0f));
        }

        if (auto animationEditor = animate<animation::SkewAnimation>(button->getAnimator(), *button->getAnimationComponent()))
        {
            animationEditor.addKeyFrame(0, vec2(1.f, .0f));
            animationEditor.addKeyFrame(100, vec2(.0f, 1.f));
        }

        return button;
    }

    nau::ui::NauButton* createButtonWithAnimatedStates(const cocos2d::Vec2& position)
    {
        using namespace nau::math;

        nau::ui::NauButtonData data;

        animation::AnimationInstanceCreationData createStopped{ .isStopped = true };

        data.defaultImageFileName = "/res/Images/nau_button_active.png";

        data.hoveredImageFileName = "/res/Images/nau_button_hover.png";
        auto hoverAnimation = rtti::createInstance<animation::ScaleAnimation>();
        if (auto animationEditor = hoverAnimation->createEditor())
        {
            animationEditor.addKeyFrame(0, { 1.f, 1.f, 1.f });
            animationEditor.addKeyFrame(240, { 2.5, 2.5, 2.5 });
        }
        data.hoveredAnimation.animation = rtti::createInstance<animation::AnimationInstance>("", hoverAnimation, &createStopped);
        data.hoveredAnimation.bPlayReversedOnExit = true;

        data.disableImageFileName = "/res/Images/nau_button_disabled.png";

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

    async::Task<> debugLoadSceneFromAsset(nau::ui::Canvas& uiCanvas)
    {
        AssetRef<> uiAssetRef{ "file:/content_ui/canvas/sample.nui" };
        nau::Ptr<ui::data::UiAssetView> view = co_await uiAssetRef.getAssetViewTyped<ui::data::UiAssetView>();

        if (view)
        {
            co_await ui::UiCanvasBuilder::loadIntoScene(&uiCanvas, view);
        }
    }

    nau::ui::NauScroll* createScroll(const cocos2d::Vec2& position)
    {
        auto scroll = nau::ui::NauScroll::create(nau::ui::NauScroll::ScrollType::vertical, { 300, 150 });
        scroll->setPosition(position);

        std::string labelText = "<color=0xFF00FF00>Richard</color>";
        auto label = nau::ui::NauLabel::create(labelText.c_str(), "/res/fonts/bitmapFontTest4.fnt",
            nau::ui::HorizontalAlignment::center,
            nau::ui::VerticalAlignment::center,
            nau::ui::NauLabel::Overflow::none,
            nau::ui::NauLabel::Wrapping::character);

        label->setContentSize(cocos2d::Size(150.0f, 50.0f));
        label->updateLabel();
        scroll->addChildWithAlignment(label);

        const int contentItemsCount = 4;
        for (int i = 0; i < contentItemsCount; i++)
        {
            std::string labelText = "<color=0xFFFF0000>Richard " + std::to_string(i) + "</color>";
            auto label = nau::ui::NauLabel::create(labelText.c_str(), "/res/fonts/bitmapFontTest4.fnt",
                nau::ui::HorizontalAlignment::center,
                nau::ui::VerticalAlignment::center,
                nau::ui::NauLabel::Overflow::none,
                nau::ui::NauLabel::Wrapping::character);

            label->setContentSize(cocos2d::Size(150.0f, 50.0f));
            label->updateLabel();
            scroll->addChildWithAlignment(label);
        }

        const int contentItemsCountNext = 7;
        for (int i = contentItemsCount + 1; i < contentItemsCountNext; i++)
        {
            std::string labelText = "<color=0xFFFF0000>Richard " + std::to_string(i) + "</color>";
            auto label = nau::ui::NauLabel::create(labelText.c_str(), "/res/fonts/bitmapFontTest4.fnt",
                nau::ui::HorizontalAlignment::center,
                nau::ui::VerticalAlignment::center,
                nau::ui::NauLabel::Overflow::none,
                nau::ui::NauLabel::Wrapping::character);

            label->setContentSize(cocos2d::Size(150.0f, 50.0f));
            label->updateLabel();
            scroll->addChildWithAlignment(label);
        }

        auto sprite = nau::ui::Sprite::create();
        sprite->initWithFile("/res/Images/nau_button_pressed.png");
        if (auto animationEditor = animate<animation::ColorAnimation>(sprite->getAnimator(), *sprite->getAnimationComponent()))
        {
            animationEditor.addKeyFrame(0, math::Color3(1.f, .0f, .0f));
            animationEditor.addKeyFrame(100, math::Color3(.0f, 1.f, .0f));
        }


        scroll->addScrollBarSprite(sprite);
        scroll->moveTo(label);

        return scroll;
    }

    async::Task<> loadSampleUi(nau::ui::Canvas* sampleUiScene)
    {
        auto engineScene = getSceneFactory().createEmptyScene();
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

        auto rectNode = nau::ui::DrawNode::create();
        math::vec2 rectangle[4];
        rectangle[0] = math::vec2 (0.f, 0.f);
        rectangle[1] = math::vec2 (100.f, 0.f);
        rectangle[2] = math::vec2 (100.f, 100.f);
        rectangle[3] = math::vec2 (0.f, 100.f);

        rectNode->drawPolygon(rectangle, 4, cocos2d::Color4F::RED, 1, cocos2d::Color4F::RED);
        nau::ui::Node* container = nau::ui::Node::create();
        container->setPosition({ 50.f, 50.f });
        container->addChild(rectNode);

        nau::ui::Sprite* sprite = nau::ui::Sprite::create("/res/Images/nau_button_active.png");
        sprite->setPosition({ 300.f, 300.f });
        sprite->enableDebugDraw(true, nau::ui::DebugDrawLevel::borders, math::Color4(0.0f, 0.0f, 1.0f, 1.0f));
        sprite->redrawDebug();
        sampleUiScene->addChild(sprite);
        sampleUiScene->addChild(container);

        auto scroll = createScroll(cocos2d::Vec2(350, 500));


        nau::ui::Sprite* scrollBgSprite = nau::ui::Sprite::create("/res/Images/YellowSquare.png");
        scrollBgSprite->setContentSize({300, 150});
        scrollBgSprite->setPosition({350, 500});
        scrollBgSprite->addChild(scroll);

        scroll->setPosition(scrollBgSprite->getContentSize() * 0.5f);
        scroll->enableDebugDraw(true, nau::ui::DebugDrawLevel::includingNestedElements);
        scroll->redrawDebug();
        
        sampleUiScene->addChild(scrollBgSprite);
        
        _scr = scroll;

        auto label = createLabel(cocos2d::Vec2(200, 200));
        label->enableDebugDraw(true);
        label->redrawDebug();

        sampleUiScene->addChild(label);
        sampleUiScene->addChild(createLabel(cocos2d::Vec2(200, 200)));

        auto button = createButton(cocos2d::Vec2(200, 400));
        button->enableDebugDraw(true, nau::ui::DebugDrawLevel::borders, math::Color4(1.0f, 0.0f, 0.0f, 1.0f));
        button->redrawDebug();
        button->getTitleLabel()->setText("SCR-V");

        button->setOnClickCallback([button]() 
        {
            _scr->setScrollType(nau::ui::NauScroll::ScrollType::vertical);
            
        });

        sampleUiScene->addChild(button);

        auto animBtn = createButtonWithAnimatedStates(cocos2d::Vec2(300, 400));
        animBtn->getTitleLabel()->setText("SCR-H");
        sampleUiScene->addChild(animBtn);

        animBtn->setOnClickCallback([animBtn]() 
        {
            _scr->setScrollType(nau::ui::NauScroll::ScrollType::horizontal);
            
        });


        nau::ui::Node* spriteAllignContainer = nau::ui::Node::create();
        if (auto animationEditor = animate<animation::TransformAnimation>(spriteAllignContainer->getAnimator(), *spriteAllignContainer->getAnimationComponent()))
        {
            animationEditor.addKeyFrame(0, math::Transform::identity());
            animationEditor.addKeyFrame(400, math::Transform(math::quat::rotationY(1.57f), math::vec3(500.f, 500.0f, .0f), math::vec3(1.f, 1.f, 1.f)));
        }


        spriteAllignContainer->setContentSize({100.0f, 100.0f});
        nau::ui::Sprite* nestedSprite = nau::ui::Sprite::create();
        nestedSprite->initWithFile("/res/Images/YellowSquare.png");
        spriteAllignContainer->addChild(nestedSprite);
        nestedSprite->setPosition(spriteAllignContainer->getContentSize() * 0.5f);

        sampleUiScene->addChild(spriteAllignContainer);

        {
            auto label = nau::ui::NauLabel::create("И вновь продолжается бой, ", "/res/fonts/OpenSans48.fnt");
            label->setPosition(math::vec2(100, 350));
            sampleUiScene->addChild(label);
        }
        {
            auto label = nau::ui::NauLabel::create("И сердцу тревожно в груди", "/res/fonts/OpenSans32.fnt");
            label->setPosition(math::vec2(100, 300));
            sampleUiScene->addChild(label);
        }
        {
            auto label = nau::ui::NauLabel::create("И Ленин - такой молодой,", "/res/fonts/OpenSans32Bold.fnt");
            label->setPosition(math::vec2(100, 250));
            sampleUiScene->addChild(label);
        }
        {
            auto label = nau::ui::NauLabel::create("И юный Октябрь впереди!", "/res/fonts/OpenSans24.fnt");
            label->setPosition(math::vec2(100, 200));
            sampleUiScene->addChild(label);
        }


        auto slider = nau::ui::NauSlider::create();
        slider->setPosition({100, 450});
        sampleUiScene->addChild(slider);

        
        slider->setTrackSprite("/res/Images/scroll_bar.png");
        slider->setThumbSprite("/res/Images/nau_button_pressed.png");

        co_await debugLoadSceneFromAsset(*sampleUiScene);

        co_await getSceneManager().activateScene(std::move(engineScene));
    }

    /**
    */
    class UiAppSampleDelegate final : public SampleAppDelegate
    {
    public:
        UiAppSampleDelegate() :
            SampleAppDelegate("uiSampleApp")
        {
        }

    private:
        Result<> initializeServices() override
        {
            return ResultSuccess;
        }

        async::Task<> startupApplication() override
        {
            nau::ui::Canvas* sampleUiScene = nau::ui::Canvas::create("canvas");
            nau::getServiceProvider().get<nau::ui::UiManager>().addCanvas(sampleUiScene);

            co_await loadSampleUi(sampleUiScene);

            nau::ui::Canvas* scrollUiScene = nau::ui::Canvas::create("scrollCanvas");
            nau::getServiceProvider().get<nau::ui::UiManager>().addCanvas(scrollUiScene);

            co_await scrollScene(scrollUiScene);
            scrollUiScene->setVisible(false);
        }

        void onApplicationStep([[maybe_unused]] std::chrono::milliseconds dt) override
        {
           using namespace std::chrono_literals;
            if(nau::input::isKeyboardButtonPressed(0, nau::input::Key::Right))
            {
                auto scrollCanvas = nau::getServiceProvider().get<nau::ui::UiManager>().getCanvas("scrollCanvas");
                scrollCanvas->setVisible(true);

                auto defaultCanvas = nau::getServiceProvider().get<nau::ui::UiManager>().getCanvas("canvas");
                defaultCanvas->setVisible(false);
            }

            if(nau::input::isKeyboardButtonPressed(0, nau::input::Key::Left))
            {
                auto defaultCanvas = nau::getServiceProvider().get<nau::ui::UiManager>().getCanvas("canvas");
                defaultCanvas->setVisible(true);

                auto scrollCanvas = nau::getServiceProvider().get<nau::ui::UiManager>().getCanvas("scrollCanvas");
                scrollCanvas->setVisible(false);
            }

            std::this_thread::sleep_for(16ms);
        }
    };


} // namespace nau::sample

int main(int argc, char** argv)
{
    using namespace nau;
    return runApplication(eastl::make_unique<sample::UiAppSampleDelegate>());
}
