// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_sprite_adapter.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

#include "pxr/base/gf/vec2i.h"
#include "pxr/base/gf/vec4d.h"

#include "nau/NauGuiSchema/nauGuiSprite.h"

#include "nau/ui/elements/sprite.h"
#include "usd_translator/ui_translator/usd_ui_node_adapter.h"

using namespace nau;
using namespace PXR_NS;

namespace UsdTranslator
{
    UsdUiSpriteAdapter::UsdUiSpriteAdapter(PXR_NS::UsdPrim prim)
    : UsdUiNodeAdapter(prim)
    , m_sprite(nullptr)
    {
    }

    bool UsdUiSpriteAdapter::isValid() const
    {
        return !!m_sprite;
    }

    void UsdUiSpriteAdapter::update()
    {
        if (!m_sprite)
        {
            return;
        }

        internalUpdate();
    }


    void UsdUiSpriteAdapter::serializeNodeContent(nau::DataBlock &blk)
    {
        UsdUiNodeAdapter::serializeNodeContent(blk);

        const GuiNauGuiSprite usdSprite{ getPrim() };

        PXR_NS::SdfAssetPath sdfPath;
        usdSprite.GetTextureTextureAttr().Get(&sdfPath);
        const auto sourcePath = getSourcePath(sdfPath);

        nau::DataBlock* spriteData = blk.addBlock("sprite_data");
        spriteData->setStr("fileName", sourcePath.c_str());
    }

    nau::Uid UsdUiSpriteAdapter::getUid() const
    {
        return id;
    }

    nau::ui::Node* UsdUiSpriteAdapter::initializeNode()
    {
        m_sprite = createSprite();
        m_node = m_sprite;
        m_sprite->retain();

        nau::getServiceProvider().get<nau::ui::UiManager>().setElementChangedCallback(id, [this](nau::ui::Node* node) 
        {
            if(id == node->getUid())
            {
                internalPrimUpdate(node);
            }
        });

        UsdUiSpriteAdapter::internalUpdate();

        return m_sprite;
    }

    nau::ui::Node* UsdUiSpriteAdapter::getNode() const
    {
        return m_sprite;
    }

    void UsdUiSpriteAdapter::addChildInternal(nau::ui::Node* node)
    {
        ((nau::ui::Node*)m_sprite)->addChild(node);
    }

    void UsdUiSpriteAdapter::internalUpdate()
    {
        UsdUiNodeAdapter::internalUpdate();

        validateDataCache();

        if(m_cachedAdapterData.m_isDirty)
        {
            const auto sourcePath = getSourcePath(m_cachedAdapterData.m_sdfTexturePath);
            m_sprite->initWithFile(sourcePath.c_str());
        }
    }

    void UsdUiSpriteAdapter::validateDataCache()
    {
         const GuiNauGuiSprite usdSprite{ getPrim() };

        SpriteAdapterData data;
        usdSprite.GetTextureTextureAttr().Get(&data.m_sdfTexturePath);

        bool isFirstInitialization = !m_cachedAdapterData.m_initialized;
        bool isDataChanged = !(m_cachedAdapterData == data);

        m_cachedAdapterData = data;
        m_cachedAdapterData.m_initialized = true;
        m_cachedAdapterData.m_isDirty = isFirstInitialization || isDataChanged;
    }

    nau::ui::Sprite* UsdUiSpriteAdapter::createSprite()
    {
        nau::ui::Sprite* sprite = nau::ui::Sprite::create();
        id = sprite->getUid();
        return sprite;
    }

    void UsdUiSpriteAdapter::destroyNode()
    {
        m_sprite->removeFromParent();
        m_sprite->release();

        nau::getServiceProvider().get<nau::ui::UiManager>().removeElementChangedCallback(id);
    }

    DEFINE_UI_TRANSLATOR(UsdUiSpriteAdapter, "NauGuiSprite"_tftoken);
}
