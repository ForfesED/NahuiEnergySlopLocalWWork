// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_node_adapter.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

#include "pxr/base/gf/vec2i.h"
#include "pxr/base/gf/vec2d.h"
#include "pxr/base/gf/vec4d.h"

#include "nau/NauGuiSchema/nauGuiNode.h"

#include "nau/ui/elements/node.h"

using namespace nau;
using namespace PXR_NS;

namespace UsdTranslator
{
    UsdUiNodeAdapter::UsdUiNodeAdapter(PXR_NS::UsdPrim prim)
    : IUIPrimAdapter(prim)
    , m_node(nullptr)
    {
    }

    bool UsdUiNodeAdapter::isValid() const
    {
        return !!m_node;
    }

    void UsdUiNodeAdapter::update()
    {
        if (!m_node)
        {
            return;
        }

        internalUpdate();
    }

    void UsdUiNodeAdapter::serializeNodeContent(nau::DataBlock &blk)
    {
        IUIPrimAdapter::serializeNodeContent(blk);

        blk.addStr("type", getType().c_str());
        const GuiNauGuiNode usdNode{ getPrim() };

        int zOrder;
        bool visible;
        int tag;
        std::string name;
        pxr::GfVec2d anchorPoint;
        pxr::GfVec2d position;
        pxr::GfVec2d contentSize;
        double rotation;
        pxr::GfVec2d scale;
        pxr::GfVec2d skew;
        pxr::GfVec2d rotationSkew;
        pxr::GfVec4d color;
        bool cascadeColorEnabled;
        bool cascadeOpacityEnabled;
        bool enableDebugDraw;

        usdNode.GetZOrderAttr().Get(&zOrder);
        usdNode.GetVisibleAttr().Get(&visible);
        usdNode.GetTagAttr().Get(&tag);
        usdNode.GetNameAttr().Get(&name);
        usdNode.GetAnchorPointAttr().Get(&anchorPoint);
        usdNode.GetPositionAttr().Get(&position);
        usdNode.GetContentSizeAttr().Get(&contentSize);
        usdNode.GetRotationAttr().Get(&rotation);
        usdNode.GetScaleAttr().Get(&scale);
        usdNode.GetSkewAttr().Get(&skew);
        usdNode.GetRotationSkewAttr().Get(&rotationSkew);
        usdNode.GetColorRGBAAttr().Get(&color);
        usdNode.GetCascadeColorEnabledAttr().Get(&cascadeColorEnabled);
        usdNode.GetCascadeOpacityEnabledAttr().Get(&cascadeOpacityEnabled);
        usdNode.GetEnableDebugDrawAttr().Get(&enableDebugDraw);

        blk.setInt("zOrder", zOrder);
        blk.setBool("visible", visible);
        blk.setStr("name", name.c_str());
        blk.setInt("tag", tag);
        blk.setPoint2("anchorPoint", {
            static_cast<float>(anchorPoint.data()[0]),
            static_cast<float>(anchorPoint.data()[1])
        });
        blk.setPoint2("translation", {
            static_cast<float>(position.data()[0]),
            static_cast<float>(position.data()[1]),
        });
        blk.setPoint2("contentSize", {
            static_cast<float>(contentSize.data()[0]),
            static_cast<float>(contentSize.data()[1])
        });
        blk.setReal("rotation", rotation);
        blk.setPoint2("scale", {
            static_cast<float>(scale.data()[0]),
            static_cast<float>(scale.data()[1]),
        });
        blk.setPoint2("skew", {
            static_cast<float>(skew.data()[0]),
            static_cast<float>(skew.data()[1])
        });
        blk.setPoint2("rotationSkew", {
            static_cast<float>(rotationSkew.data()[0]),
            static_cast<float>(rotationSkew.data()[1])
        });
        blk.setE3dcolor("color", {
            static_cast<unsigned char>(color.data()[0] * 255.),
            static_cast<unsigned char>(color.data()[1] * 255.),
            static_cast<unsigned char>(color.data()[2] * 255.),
            static_cast<unsigned char>(color.data()[3] * 255.)
        });
        blk.setBool("cascadeColorEnabled", cascadeColorEnabled);
        blk.setBool("cascadeOpacityEnabled", cascadeOpacityEnabled);
        blk.setBool("enableDebugDraw", enableDebugDraw);
    }

    nau::Uid UsdUiNodeAdapter::getUid() const
    {
        return id;
    }

    nau::ui::Node* UsdUiNodeAdapter::initializeNode()
    {
        m_node = createNode();
        m_node->retain();

        nau::getServiceProvider().get<nau::ui::UiManager>().setElementChangedCallback(id, [this](nau::ui::Node* node) 
        {
            if(id == node->getUid())
            {
                internalPrimUpdate(node);
            }
        });

        UsdUiNodeAdapter::internalUpdate();

        return m_node;
    }

    nau::ui::Node* UsdUiNodeAdapter::getNode() const
    {
        return m_node;
    }

    void UsdUiNodeAdapter::addChildInternal(nau::ui::Node* node)
    {
        m_node->addChild(node);
    }

    void UsdUiNodeAdapter::internalUpdate()
    {
        const GuiNauGuiNode usdNode{ getPrim() };

        // zOrder
        int zOrder;
        usdNode.GetZOrderAttr().Get(&zOrder);
        m_node->setZOrder(zOrder);

        // visible
        bool visible;
        usdNode.GetVisibleAttr().Get(&visible);
        m_node->setVisible(visible);

        // tag
        int tag;
        usdNode.GetTagAttr().Get(&tag);
        m_node->setTag(tag);

        // name
        std::string name;
        usdNode.GetNameAttr().Get(&name);
        m_node->nau_setName(name.c_str());

        // anchorPoint
        pxr::GfVec2d anchorPoint;
        usdNode.GetAnchorPointAttr().Get(&anchorPoint);
        m_node->setAnchorPoint(nau::math::vec2(anchorPoint.data()[0], anchorPoint.data()[1]));

        // position
        pxr::GfVec2d position;
        usdNode.GetPositionAttr().Get(&position);
        m_node->setPosition(nau::math::vec2(position.data()[0], position.data()[1]));

        // contentSize
        pxr::GfVec2d contentSize;
        usdNode.GetContentSizeAttr().Get(&contentSize);
        m_node->setContentSize(nau::math::vec2(contentSize.data()[0], contentSize.data()[1]));

        // rotation
        double rotation;
        usdNode.GetRotationAttr().Get(&rotation);
        m_node->setRotation(rotation);

        // scale
        pxr::GfVec2d scale;
        usdNode.GetScaleAttr().Get(&scale);
        m_node->setScale(scale.data()[0], scale.data()[1]);

        // skew
        pxr::GfVec2d skew;
        usdNode.GetSkewAttr().Get(&skew);
        m_node->setSkewX(skew.data()[0]);
        m_node->setSkewY(skew.data()[1]);

        // rotationSkew
        pxr::GfVec2d rotationSkew;
        usdNode.GetRotationSkewAttr().Get(&rotationSkew);
        auto rSkewX = rotationSkew.data()[0];
        auto rSkewY = rotationSkew.data()[1];
        if (rSkewX != 0 || rSkewY != 0) {
            m_node->setRotationSkewX(rSkewX);
            m_node->setRotationSkewY(rSkewY);
        }

        // color
        pxr::GfVec4d color;
        usdNode.GetColorRGBAAttr().Get(&color);
        m_node->setOpacity(color.data()[3] * 255.0f);
        m_node->setColor(nau::math::E3DCOLOR(color.data()[0] * 255.0f, color.data()[1] * 255.0f, color.data()[2] * 255.0f));

        // cascadeColorEnabled
        bool cascadeColorEnabled;
        usdNode.GetCascadeColorEnabledAttr().Get(&cascadeColorEnabled);
        m_node->setCascadeColorEnabled(cascadeColorEnabled);

        // cascadeOpacityEnabled
        bool cascadeOpacityEnabled;
        usdNode.GetCascadeOpacityEnabledAttr().Get(&cascadeOpacityEnabled);
        m_node->setCascadeOpacityEnabled(cascadeOpacityEnabled);

        // enableDebugDraw
        bool enableDebugDraw;
        usdNode.GetEnableDebugDrawAttr().Get(&enableDebugDraw);
        m_node->enableDebugDraw(enableDebugDraw);
    }

    void UsdUiNodeAdapter::internalPrimUpdate(nau::ui::Node* node)
    {
        GuiNauGuiNode usdNode{ getPrim() };

        pxr::GfVec2d originalPosition;
        usdNode.GetPositionAttr().Get(&originalPosition);
        pxr::GfVec2d position(node->getPositionX(), node->getPositionY());
        
        if(originalPosition != position)
        {
            usdNode.GetPositionAttr().Set(position);
        }

        pxr::GfVec2d originalContentSize;
        usdNode.GetContentSizeAttr().Get(&originalContentSize);
        pxr::GfVec2d contentSize(node->getContentSize().getX(), node->getContentSize().getY());
        if(originalContentSize != contentSize)
        {
            usdNode.GetContentSizeAttr().Set(contentSize);
        }

        // zOrder
        int originalZOrder;
        usdNode.GetZOrderAttr().Get(&originalZOrder);
        if(originalZOrder != node->geZOrder())
        {
            usdNode.GetZOrderAttr().Set(node->geZOrder());
        }

        // visible
        bool originalVisible;
        usdNode.GetVisibleAttr().Get(&originalVisible);
        if(originalVisible != node->isVisible())
        {
            usdNode.GetVisibleAttr().Set(node->isVisible());
        }

        // tag
        int originalTag;
        usdNode.GetTagAttr().Get(&originalTag);
        if(originalTag != node->getTag())
        {
            usdNode.GetTagAttr().Set(node->getTag());
        }

        //name
        std::string originalName;
        usdNode.GetNameAttr().Get(&originalName);
        if (originalName.c_str() != node->getName())
        {
            usdNode.GetNameAttr().Set(std::string(node->getName().c_str()));
        }

        // anchorPoint
        pxr::GfVec2d originalAnchorPoint;
        usdNode.GetAnchorPointAttr().Get(&originalAnchorPoint);
        pxr::GfVec2d anchorPoint(node->getAnchorPoint().getX(), node->getAnchorPoint().getY());
        if (originalAnchorPoint != anchorPoint)
        {
            usdNode.GetAnchorPointAttr().Set(anchorPoint);
        }

        // rotation
        double originalRotation;
        usdNode.GetRotationAttr().Get(&originalRotation);
        if (originalRotation != node->getRotation())
        {
            usdNode.GetRotationAttr().Set(node->getRotation());
        }

        // scale
        pxr::GfVec2d originalScale;
        usdNode.GetScaleAttr().Get(&originalScale);
        pxr::GfVec2d scale(node->getScaleX(), node->getScaleY());
        if (originalScale != scale)
        {
            usdNode.GetScaleAttr().Set(scale);
        }

        // skew
        pxr::GfVec2d originalSkew;
        usdNode.GetSkewAttr().Get(&originalSkew);
        pxr::GfVec2d skew(node->getSkewX(), node->getSkewY());
        if (originalSkew != skew)
        {
            usdNode.GetSkewAttr().Set(skew);
        }

        // rotationSkew
        pxr::GfVec2d originalRotationSkew;
        usdNode.GetRotationSkewAttr().Get(&originalRotationSkew);
        pxr::GfVec2d rotationSkew(node->getRotationSkewX(), node->getRotationSkewY());
        if (originalRotationSkew != rotationSkew)
        {
            usdNode.GetRotationSkewAttr().Set(rotationSkew);
        }

        // color
        pxr::GfVec4d originalColor;
        usdNode.GetColorRGBAAttr().Get(&originalColor);
        pxr::GfVec4d color(
            node->getColor().r / 255.0,
            node->getColor().g / 255.0,
            node->getColor().b / 255.0,
            node->getOpacity() / 255.0);
        if (originalColor != color)
        {
            usdNode.GetColorRGBAAttr().Set(color);
        }

        // cascadeColorEnabled
        bool originalCascadeColorEnabled;
        usdNode.GetCascadeColorEnabledAttr().Get(&originalCascadeColorEnabled);
        if (originalCascadeColorEnabled != node->isCascadeColorEnabled())
        {
            usdNode.GetCascadeColorEnabledAttr().Set(node->isCascadeColorEnabled());
        }

        // cascadeOpacityEnabled
        bool originalCascadeOpacityEnabled;
        usdNode.GetCascadeOpacityEnabledAttr().Get(&originalCascadeOpacityEnabled);
        if (originalCascadeOpacityEnabled != node->isCascadeOpacityEnabled())
        {
            usdNode.GetCascadeOpacityEnabledAttr().Set(node->isCascadeOpacityEnabled());
        }
    }

    nau::ui::Node* UsdUiNodeAdapter::createNode()
    {
        auto node = nau::ui::Node::create();
        id = node->getUid();
        return node;
    }

    void UsdUiNodeAdapter::destroyNode()
    {
        m_node->removeFromParent();
        m_node->release();

        nau::getServiceProvider().get<nau::ui::UiManager>().removeElementChangedCallback(id);
    }

    DEFINE_UI_TRANSLATOR(UsdUiNodeAdapter, "NauGuiNode"_tftoken);
}
