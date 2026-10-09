// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_layer_adapter.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

#include "pxr/base/gf/vec2i.h"

#include "nau/NauGuiSchema/nauGuiLayer.h"

#include "nau/ui/elements/layer.h"

using namespace nau;
using namespace PXR_NS;

namespace UsdTranslator
{
    UsdUiLayerAdapter::UsdUiLayerAdapter(PXR_NS::UsdPrim prim)
    : UsdUiNodeAdapter(prim)
    , m_layer(nullptr)
    {
    }

    bool UsdUiLayerAdapter::isValid() const
    {
        return !!m_layer;
    }

    void UsdUiLayerAdapter::update()
    {
        if (!m_layer)
        {
            return;
        }

        internalUpdate();
    }

    nau::Uid UsdUiLayerAdapter::getUid() const
    {
        return id;
    }

    nau::ui::Node* UsdUiLayerAdapter::initializeNode()
    {
        m_layer = createLayer();
        m_node = m_layer;
        m_layer->retain();

        nau::getServiceProvider().get<nau::ui::UiManager>().setElementChangedCallback(id, [this](nau::ui::Node* node) 
        {
            if(id == node->getUid())
            {
                internalPrimUpdate(node);
            }
        });

        UsdUiLayerAdapter::internalUpdate();

        return m_layer;
    }

    nau::ui::Node* UsdUiLayerAdapter::getNode() const
    {
        return m_layer;
    }

    void UsdUiLayerAdapter::addChildInternal(nau::ui::Node* node)
    {
        m_layer->addChild(node);
    }

    void UsdUiLayerAdapter::internalUpdate()
    {
        const GuiNauGuiLayer usdLayer{ getPrim() };

        UsdUiNodeAdapter::internalUpdate();

    }

    nau::ui::Layer* UsdUiLayerAdapter::createLayer()
    {
        auto layer = nau::ui::Layer::create();
        id = layer->getUid();
        return layer;
    }

    void UsdUiLayerAdapter::destroyNode()
    {
        m_layer->removeFromParent();
        m_layer->release();
    }

    DEFINE_UI_TRANSLATOR(UsdUiLayerAdapter, "NauGuiLayer"_tftoken);
}
