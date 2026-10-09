// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/ui/assets/ui_asset.h"

#include "nau/assets/ui_asset_accessor.h"
#include "nau/diag/assertion.h"
#include "nau/math/dag_color.h"
#include "nau/math/math.h"
#include "nau/ui/elements/layer.h"
#include "nau/ui/ui_control.h"
#include "nau/ui/button.h"
#include "nau/ui/elements/sprite.h"
#include "nau/ui/label.h"
#include "nau/ui/elements/canvas.h"
#include "nau/ui/scroll.h"
#include "nau/animation/playback/animation_instance.h"

namespace nau::ui::data
{
    namespace
    {
        ui::Node* createNode(const UiElementAssetData& elementData)
        {
            return ui::Node::create();
        }

        ui::Node* createLabel(const UiElementAssetData& elementData)
        {
            if (const auto* data = static_cast<NauLabelAssetData*>(elementData.customData.get()))
            {
                auto* label = NauLabel::create(
                    data->text,
                    data->fontRef.c_str(),
                    static_cast<HorizontalAlignment>(data->horizontalAlignment),
                    static_cast<VerticalAlignment>(data->verticalAlignment),
                    static_cast<NauLabel::Overflow>(data->overflow),
                    static_cast<NauLabel::Wrapping>(data->wrapping));

                return label;
            }

            return nullptr;
        }

        ui::Node* createButton(const UiElementAssetData& elementData)
        {
            if (const auto* data = static_cast<NauButtonAssetData*>(elementData.customData.get()))
            {
                NauButtonData buttonData;

                buttonData.defaultColor = data->normalStateData.color;
                buttonData.defaultScale = data->normalStateData.scale;
                buttonData.defaultImageFileName = data->normalStateData.imageFileName.c_str();
                if (data->normalStateData.animationAsset)
                {
                    buttonData.normalAnimation.animation = rtti::createInstance<animation::AnimationInstance>("", data->normalStateData.animationAsset);
                }

                buttonData.clickedColor = data->pressedStateData.color;
                buttonData.clickedScale = data->pressedStateData.scale;
                buttonData.clickedImageFileName = data->pressedStateData.imageFileName.c_str();
                if (data->pressedStateData.animationAsset)
                {
                    buttonData.clickedAnimation.animation = rtti::createInstance<animation::AnimationInstance>("", data->pressedStateData.animationAsset);
                }

                buttonData.hoveredColor = data->hoveredStateData.color;
                buttonData.hoveredScale = data->hoveredStateData.scale;
                buttonData.hoveredImageFileName = data->hoveredStateData.imageFileName.c_str();
                if (data->hoveredStateData.animationAsset)
                {
                    buttonData.hoveredAnimation.animation = rtti::createInstance<animation::AnimationInstance>("", data->hoveredStateData.animationAsset);
                }

                buttonData.disableColor = data->disabledStateData.color;
                buttonData.disableScale = data->disabledStateData.scale;
                buttonData.disableImageFileName = data->disabledStateData.imageFileName.c_str();
                if (data->disabledStateData.animationAsset)
                {
                    buttonData.disabledAnimation.animation = rtti::createInstance<animation::AnimationInstance>("", data->disabledStateData.animationAsset);
                }

                auto* button = NauButton::create(buttonData);
                
                return button;
            }

            return nullptr;
        }

        ui::Node* createSprite(const UiElementAssetData& elementData)
        {
            if (const auto* data = static_cast<SpriteAssetData*>(elementData.customData.get()))
            {
                return Sprite::create(data->fileName.c_str());
            }

            return nullptr;
        }

        ui::Node* createScroll(const UiElementAssetData& elementData)
        {
            if (const auto* data = static_cast<ScrollAssetData*>(elementData.customData.get()))
            {
                const ui::NauScroll::ScrollType scrollType = data->scrollType == "horizontal" 
                    ? ui::NauScroll::ScrollType::horizontal
                    : ui::NauScroll::ScrollType::vertical;
                return NauScroll::create(scrollType);
            }
            return nullptr;
        }

        ui::Node* createDrawNode(const UiElementAssetData& elementData)
        {
            if (const auto* data = static_cast<DrawNodeAssetData*>(elementData.customData.get()))
            {
                DrawNode* node = DrawNode::create();
                node->drawPolygon(
                    data->drawPolygon.points,
                    sizeof(data->drawPolygon.points) / sizeof(data->drawPolygon.points[0]),
                    data->drawPolygon.fillColor,
                    data->drawPolygon.borderWidth,
                    data->drawPolygon.borderColor
                );
                return node;
            }
            return nullptr;
        }

        ui::Node* createLayer(const UiElementAssetData& elementData)
        {
            return Layer::create();
        }

        ui::Node* createUiNode(const UiElementAssetData& elementData)
        {
            ui::Node* newNode = nullptr;

            switch (elementData.elementType)
            {
            case UiElementType::Node:
                newNode = createNode(elementData);
                break;
            case UiElementType::Label:
                newNode = createLabel(elementData);
                break;
            case UiElementType::Button:
                newNode = createButton(elementData);
                break;
            case UiElementType::Sprite:
                newNode = createSprite(elementData);
                break;
            case UiElementType::Scroll:
                newNode = createScroll(elementData);
                break;
            case UiElementType::DrawNode:
                newNode = createDrawNode(elementData);
                break;
            case UiElementType::Layer:
                newNode = createLayer(elementData);
                break;
            default:
                NAU_FAILURE("Unknown node type {}", static_cast<int>(elementData.elementType));
            }

            if (newNode)
            {
                newNode->setPosition(elementData.translation);
                newNode->setScale(elementData.scale.getX(), elementData.scale.getY());

                if (elementData.rotation != 0.f)
                {
                    newNode->setRotation(elementData.rotation);
                }

                newNode->nau_setName(elementData.name);
                newNode->setZOrder(elementData.zOrder);
                newNode->setVisible(elementData.visible);
                newNode->setAnchorPoint(elementData.anchorPoint);
                newNode->setContentSize(elementData.contentSize);
                newNode->setSkewX(elementData.scew.getX());
                newNode->setSkewY(elementData.scew.getY());

                if (elementData.rotationSkew.getX() != 0.f)
                {
                    newNode->setRotationSkewX(elementData.rotationSkew.getX());
                }

                if (elementData.rotationSkew.getY())
                {
                    newNode->setRotationSkewY(elementData.rotationSkew.getY());
                }

                newNode->setColor(elementData.color);
                newNode->setCascadeColorEnabled(elementData.cascadeColorEnabled);
                newNode->setCascadeOpacityEnabled(elementData.cascadeOpacityEnabled);
                newNode->enableDebugDraw(elementData.enableDebugDraw);
            }
            else
            {
                NAU_FAILURE("Failed to create element of type {}", static_cast<int>(elementData.elementType));
            }

            return newNode;
        }

        void createUiNodeHierarchy(ui::Node* parent, const UiElementAssetData& elementData)
        {
            if (ui::Node* newNode = createUiNode(elementData))
            {
                for (const auto& childData : elementData.children)
                {
                    createUiNodeHierarchy(newNode, childData);
                }

                bool isAdded = false;
                // todo: Change button API to get rid of this
                if (auto* parentButton = ui::Node::cast<NauButton>(parent))
                {
                    if (auto* buttonTitle = ui::Node::cast<NauLabel>(newNode))
                    {
                        parentButton->setTitleLabel(buttonTitle);
                        isAdded = true;
                    }
                }

                if (!isAdded)
                {
                    parent->addChild(newNode);
                }
            }
        }
    }

    nau::async::Task<nau::Ptr<UiAssetView>> UiAssetView::createFromAssetAccessor(nau::Ptr<> accessor)
    {
        using namespace nau::async;

        NAU_ASSERT(accessor);

        ASYNC_SWITCH_EXECUTOR(async::Executor::getDefault());

        auto& uiSceneAccessor = accessor->as<IUiAssetAccessor&>();
        auto instancePtr = rtti::createInstance<UiAssetView>();
        UiAssetView* instance = instancePtr.get();

        co_await uiSceneAccessor.copyUiElements(instance->m_uiElementsData);

        co_return instancePtr;
    }

    void UiAssetView::createUi(Canvas* uiCanvas) const
    {
        for (const auto& elementData : m_uiElementsData)
        {
            createUiNodeHierarchy(uiCanvas, elementData);
        }
    }

} // namespace nau::ui::data
