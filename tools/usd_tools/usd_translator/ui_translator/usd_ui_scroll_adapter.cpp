// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_scroll_adapter.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

#include "pxr/base/gf/vec2i.h"

#include "nau/NauGuiSchema/nauGuiScroll.h"

#include "nau/ui/scroll.h"

using namespace nau;
using namespace PXR_NS;

namespace UsdTranslator
{
    UsdUiScrollAdapter::UsdUiScrollAdapter(PXR_NS::UsdPrim prim)
    : UsdUiNodeAdapter(prim)
    , m_scroll(nullptr)
    {
    }

    bool UsdUiScrollAdapter::isValid() const
    {
        return !!m_scroll;
    }

    void UsdUiScrollAdapter::update()
    {
        if (!m_scroll)
        {
            return;
        }

        internalUpdate();
    }

    nau::Uid UsdUiScrollAdapter::getUid() const
    {
        return id;
    }

    nau::ui::Node* UsdUiScrollAdapter::initializeNode()
    {
        m_scroll = createScroll();
        m_node = m_scroll;
        m_scroll->retain();

        nau::getServiceProvider().get<nau::ui::UiManager>().setElementChangedCallback(id, [this](nau::ui::Node* node) 
        {
            if(id == node->getUid())
            {
                internalPrimUpdate(node);
            }
        });

        UsdUiScrollAdapter::internalUpdate();

        return m_scroll;
    }

    nau::ui::Node* UsdUiScrollAdapter::getNode() const
    {
        return m_scroll;
    }

    void UsdUiScrollAdapter::addChildInternal(nau::ui::Node* node)
    {
        // Zeroing the object's position
        node->setPosition({ 0, 0 });
        m_scroll->addChildWithAlignment(node);

        // TODO: moveTo is working strangely now. Disclose after edits related to window resizing.
        //m_scroll->moveTo(node);
    }

    void UsdUiScrollAdapter::internalUpdate()
    {
        const GuiNauGuiScroll usdScroll{ getPrim() };

        UsdUiNodeAdapter::internalUpdate();

        pxr::GfVec2d contentRootSize;
        usdScroll.GetContentRootSizeAttr().Get(&contentRootSize);
        m_scroll->setContentRootSize(nau::math::vec2(contentRootSize.data()[0], contentRootSize.data()[1]));

        PXR_NS::SdfAssetPath sdfPath;
        usdScroll.GetTextureDefaultScrollImageAttr().Get(&sdfPath);
        const auto sourcePath = getSourcePath(sdfPath);
        m_scroll->addScrollBarSprite(sourcePath);
    }

    void UsdUiScrollAdapter::internalPrimUpdate(nau::ui::Node* node)
    {
        NAU_LOG_INFO("SCROLL NEED BE UPDATED");

        const GuiNauGuiScroll usdScroll{ getPrim() };

        pxr::GfVec2d originalContentRootSize;
        usdScroll.GetContentRootSizeAttr().Get(&originalContentRootSize);
        pxr::GfVec2d contentRootSize(m_scroll->getContentRootSize().getX(), m_scroll->getContentRootSize().getY());
        if(originalContentRootSize != contentRootSize)
        {
            usdScroll.GetContentSizeAttr().Set(contentRootSize);
        }

        pxr::GfVec2d originalContentRootPosition;
        usdScroll.GetContentRootPositionAttr().Get(&originalContentRootPosition);
        pxr::GfVec2d contentRootPosition(m_scroll->getContentRootPosition().getX(), m_scroll->getContentRootPosition().getY());
        if(originalContentRootPosition != contentRootPosition)
        {
            usdScroll.GetContentRootPositionAttr().Set(contentRootPosition);
        }


        UsdUiNodeAdapter::internalPrimUpdate(node);
    }

    nau::ui::NauScroll* UsdUiScrollAdapter::createScroll()
    {
        // TODO: Make default creator in the future...
        auto scroll = nau::ui::NauScroll::create(nau::ui::NauScroll::ScrollType::vertical, { 400, 600 });
        scroll->retain();

        auto sprite = nau::ui::Sprite::create("/res/Images/Slider-default.png");
        scroll->addScrollBarSprite(sprite);

        id = scroll->getUid();

        return scroll;
    }

    void UsdUiScrollAdapter::destroyNode()
    {
        m_scroll->removeFromParent();
        m_scroll->release();

        nau::getServiceProvider().get<nau::ui::UiManager>().removeElementChangedCallback(id);
    }

    DEFINE_UI_TRANSLATOR(UsdUiScrollAdapter, "NauGuiScroll"_tftoken);
}
