// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_draw_node_adapter.h"
#include "nau/dataBlock/dag_dataBlock.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

#include "pxr/base/gf/vec2i.h"
#include "pxr/base/gf/vec4d.h"

#include "nau/NauGuiSchema/nauGuiDrawNode.h"

#include "nau/ui/elements/draw_node.h"

using namespace nau;
using namespace PXR_NS;

namespace UsdTranslator
{
    UsdUiDrawNodeAdapter::UsdUiDrawNodeAdapter(PXR_NS::UsdPrim prim)
    : UsdUiNodeAdapter(prim)
    , m_drawNode(nullptr)
    {
    }

    bool UsdUiDrawNodeAdapter::isValid() const
    {
        return !!m_drawNode;
    }

    void UsdUiDrawNodeAdapter::update()
    {
        if (!m_drawNode)
        {
            return;
        }
        m_drawNode->clearDrawNode();
        internalUpdate();
    }


    void UsdUiDrawNodeAdapter::serializeNodeContent(nau::DataBlock &blk)
    {
        UsdUiNodeAdapter::serializeNodeContent(blk);

        {
            DataBlock* polygon = blk.addBlock("draw_polygon");
            pxr::GfVec2i point0;
            pxr::GfVec2i point1;
            pxr::GfVec2i point2;
            pxr::GfVec2i point3;

            pxr::GfVec4d fillColor;
            pxr::GfVec4d borderColor;

            int borderWidht;

            const GuiNauGuiDrawNode usdDrawNode{ getPrim() };

            usdDrawNode.GetBottomLeftCornerAttr().Get(&point0);
            usdDrawNode.GetUpperLeftCornerAttr().Get(&point1);
            usdDrawNode.GetBottomRightCornerAttr().Get(&point2);
            usdDrawNode.GetUpperRightCornerAttr().Get(&point3);

            usdDrawNode.GetFillColorRGBAAttr().Get(&fillColor);
            usdDrawNode.GetBorderColorRGBAAttr().Get(&borderColor);
            usdDrawNode.GetBorderWidthAttr().Get(&borderWidht);

            polygon->addPoint2("point0", {static_cast<float>(point0.data()[0]), static_cast<float>(point0.data()[1])});
            polygon->addPoint2("point1", {static_cast<float>(point1.data()[0]), static_cast<float>(point1.data()[1])});
            polygon->addPoint2("point2", {static_cast<float>(point2.data()[0]), static_cast<float>(point2.data()[1])});
            polygon->addPoint2("point3", {static_cast<float>(point3.data()[0]), static_cast<float>(point3.data()[1])});

            polygon->addPoint4("fill_color",
                {
                    static_cast<float>(fillColor.data()[0]),
                    static_cast<float>(fillColor.data()[1]),
                    static_cast<float>(fillColor.data()[2]),
                    static_cast<float>(fillColor.data()[3])
                }
            );
            polygon->addPoint4("border_color",
                {
                    static_cast<float>(borderColor.data()[0]),
                    static_cast<float>(borderColor.data()[1]),
                    static_cast<float>(borderColor.data()[2]),
                    static_cast<float>(borderColor.data()[3])
                }
            );
            polygon->setReal("border_width", borderWidht);
        }
    }

    nau::Uid UsdUiDrawNodeAdapter::getUid() const
    {
        return id;
    }

    nau::ui::Node* UsdUiDrawNodeAdapter::initializeNode()
    {
        m_drawNode = nau::ui::DrawNode::create();
        m_node = m_drawNode;
        m_drawNode->retain();
        id = m_drawNode->getUid();

        nau::getServiceProvider().get<nau::ui::UiManager>().setElementChangedCallback(id, [this](nau::ui::Node* node) 
        {
            if(id == node->getUid())
            {
                internalPrimUpdate(node);
            }
        });

        UsdUiDrawNodeAdapter::internalUpdate();

        return m_drawNode;
    }

    nau::ui::Node* UsdUiDrawNodeAdapter::getNode() const
    {
        return m_drawNode;
    }

    void UsdUiDrawNodeAdapter::addChildInternal(nau::ui::Node* node)
    {
        m_drawNode->addChild(node);
    }

    void UsdUiDrawNodeAdapter::internalUpdate()
    {
        UsdUiNodeAdapter::internalUpdate();

        // Getting usd attributes
        pxr::GfVec2i point0;
        pxr::GfVec2i point1;
        pxr::GfVec2i point2;
        pxr::GfVec2i point3;

        pxr::GfVec4d fillColor;
        pxr::GfVec4d borderColor;

        int borderWidht;

        const GuiNauGuiDrawNode usdDrawNode{ getPrim() };

        usdDrawNode.GetBottomLeftCornerAttr().Get(&point0);
        usdDrawNode.GetUpperLeftCornerAttr().Get(&point1);
        usdDrawNode.GetBottomRightCornerAttr().Get(&point2);
        usdDrawNode.GetUpperRightCornerAttr().Get(&point3);

        usdDrawNode.GetFillColorRGBAAttr().Get(&fillColor);
        usdDrawNode.GetBorderColorRGBAAttr().Get(&borderColor);
        usdDrawNode.GetBorderWidthAttr().Get(&borderWidht);

        // Converts data from usd to ui data types
        nau::math::vec2 rectangle[4];
        rectangle[0] = nau::math::vec2(point0.data()[0], point0.data()[1]);
        rectangle[1] = nau::math::vec2(point2.data()[0], point2.data()[1]);
        rectangle[2] = nau::math::vec2(point3.data()[0], point3.data()[1]);
        rectangle[3] = nau::math::vec2(point1.data()[0], point1.data()[1]);

        auto uiFillColor = nau::math::Color4(fillColor.data()[0], fillColor.data()[1], fillColor.data()[2], fillColor.data()[3]);
        auto uiBorderColor = nau::math::Color4(borderColor.data()[0], borderColor.data()[1], borderColor.data()[2], borderColor.data()[3]);

        // TODO: Add selection of drawing mode
        m_drawNode->drawPolygon(rectangle, 4, uiFillColor, borderWidht, uiBorderColor);
    }

    void UsdUiDrawNodeAdapter::destroyNode()
    {
        m_drawNode->removeFromParent();
        m_drawNode->release();
    }

    DEFINE_UI_TRANSLATOR(UsdUiDrawNodeAdapter, "NauGuiDrawNode"_tftoken);
}
