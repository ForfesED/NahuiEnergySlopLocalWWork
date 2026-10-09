// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_button_adapter.h"
#include "nau/dataBlock/dag_dataBlock.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

#include "pxr/base/gf/vec2i.h"

#include "nau/NauGuiSchema/nauGuiButton.h"

#include "nau/ui/button.h"
#include "nau/ui/button_data.h"

using namespace nau;
using namespace PXR_NS;

namespace UsdTranslator
{
    UsdUiButtonAdapter::UsdUiButtonAdapter(PXR_NS::UsdPrim prim)
    : UsdUiNodeAdapter(prim)
    , m_button(nullptr)
    {
    }

    bool UsdUiButtonAdapter::isValid() const
    {
        return !!m_button;
    }

    void UsdUiButtonAdapter::update()
    {
        if (!m_button)
        {
            return;
        }

        internalUpdate();
    }

    void UsdUiButtonAdapter::serializeNodeContent(nau::DataBlock &blk)
    {
        UsdUiNodeAdapter::serializeNodeContent(blk);

        const GuiNauGuiButton usdButton{ getPrim() };

        PXR_NS::SdfAssetPath sdfPathDefaultImage;
        PXR_NS::SdfAssetPath sdfPathHoveredImage;
        PXR_NS::SdfAssetPath sdfPathClickedImage;
        PXR_NS::SdfAssetPath sdfPathDisableImage;

        usdButton.GetTextureDefaultImageAttr().Get(&sdfPathDefaultImage);
        usdButton.GetTextureHoveredImageAttr().Get(&sdfPathHoveredImage);
        usdButton.GetTextureClickedImageAttr().Get(&sdfPathClickedImage);
        usdButton.GetTextureDisableImageAttr().Get(&sdfPathDisableImage);

        const std::string sourceDefaultImagePath = getSourcePath(sdfPathDefaultImage);
        const std::string sourceHoveredImagePath = getSourcePath(sdfPathHoveredImage);
        const std::string sourceClickedImagePath = getSourcePath(sdfPathClickedImage);
        const std::string sourceDisableImagePath = getSourcePath(sdfPathDisableImage);

        double defaultImageScale;
        double hoveredImageScale;
        double clickedImageScale;
        double disableImageScale;

        usdButton.GetDefaultScaleAttr().Get(&defaultImageScale);
        usdButton.GetHoveredScaleAttr().Get(&hoveredImageScale);
        usdButton.GetClickedScaleAttr().Get(&clickedImageScale);
        usdButton.GetDisableScaleAttr().Get(&disableImageScale);

        pxr::GfVec4d usdDefaultColor;
        pxr::GfVec4d usdHoveredColor;
        pxr::GfVec4d usdClickedColor;
        pxr::GfVec4d usdDisableColor;

        usdButton.GetDefaultColorAttr().Get(&usdDefaultColor);
        usdButton.GetHoveredColorAttr().Get(&usdHoveredColor);
        usdButton.GetClickedColorAttr().Get(&usdClickedColor);
        usdButton.GetDisableColorAttr().Get(&usdDisableColor);

        DataBlock* buttonData = blk.addBlock("button_data");

        DataBlock* normalState = buttonData->addBlock("normal");
        normalState->setStr("image", sourceDefaultImagePath.c_str());
        normalState->setReal("scale", defaultImageScale);
        normalState->setPoint4("color", {
            static_cast<float>(usdDefaultColor.data()[0]),
            static_cast<float>(usdDefaultColor.data()[1]),
            static_cast<float>(usdDefaultColor.data()[2]),
            static_cast<float>(usdDefaultColor.data()[3])
        });

        DataBlock* hoveredState = buttonData->addBlock("hovered");
        hoveredState->setStr("image", sourceHoveredImagePath.c_str());
        hoveredState->setReal("scale", hoveredImageScale);
        hoveredState->setPoint4("color", {
            static_cast<float>(usdHoveredColor.data()[0]),
            static_cast<float>(usdHoveredColor.data()[1]),
            static_cast<float>(usdHoveredColor.data()[2]),
            static_cast<float>(usdHoveredColor.data()[3])
        });

        DataBlock* pressedState = buttonData->addBlock("pressed");
        pressedState->setStr("image", sourceClickedImagePath.c_str());
        pressedState->setReal("scale", clickedImageScale);
        pressedState->setPoint4("color", {
            static_cast<float>(usdClickedColor.data()[0]),
            static_cast<float>(usdClickedColor.data()[1]),
            static_cast<float>(usdClickedColor.data()[2]),
            static_cast<float>(usdClickedColor.data()[3])
        });

        DataBlock* disabledState = buttonData->addBlock("disabled");
        disabledState->setStr("image", sourceDisableImagePath.c_str());
        disabledState->setReal("scale", disableImageScale);
        disabledState->setPoint4("color", {
            static_cast<float>(usdDisableColor.data()[0]),
            static_cast<float>(usdDisableColor.data()[1]),
            static_cast<float>(usdDisableColor.data()[2]),
            static_cast<float>(usdDisableColor.data()[3])
        });
    }

    nau::Uid UsdUiButtonAdapter::getUid() const
    {
        return id;
    }

    nau::ui::Node* UsdUiButtonAdapter::initializeNode()
    {
        m_button = createButton();
        m_node = m_button;
        m_button->retain();

        nau::getServiceProvider().get<nau::ui::UiManager>().setElementChangedCallback(id, [this](nau::ui::Node* node) 
        {
            if(id == node->getUid())
            {
                internalPrimUpdate(node);
            }
        });

        UsdUiButtonAdapter::internalUpdate();

        return m_button;
    }

    nau::ui::Node* UsdUiButtonAdapter::getNode() const
    {
        return m_button;
    }

    void UsdUiButtonAdapter::addChildInternal(nau::ui::Node* node)
    {
        m_button->addChild(node);
    }

    void UsdUiButtonAdapter::internalUpdate()
    {
        UsdUiNodeAdapter::internalUpdate();

        validateButtonDataCache();
        
        if(m_cachedButtonAdapterData.m_isDirty)
        {
            nau::ui::NauButtonData data;

            data.defaultImageFileName = getSourcePath(m_cachedButtonAdapterData.m_sdfPathDefaultImage);
            data.hoveredImageFileName = getSourcePath(m_cachedButtonAdapterData.m_sdfPathHoveredImage);
            data.clickedImageFileName = getSourcePath(m_cachedButtonAdapterData.m_sdfPathClickedImage);
            data.disableImageFileName = getSourcePath(m_cachedButtonAdapterData.m_sdfPathDisableImage);

            data.defaultColor = nau::math::Color4(m_cachedButtonAdapterData.m_usdDefaultColor.data()[0], m_cachedButtonAdapterData.m_usdDefaultColor.data()[1], m_cachedButtonAdapterData.m_usdDefaultColor.data()[2], m_cachedButtonAdapterData.m_usdDefaultColor.data()[3]);
            data.hoveredColor = nau::math::Color4(m_cachedButtonAdapterData.m_usdHoveredColor.data()[0], m_cachedButtonAdapterData.m_usdHoveredColor.data()[1], m_cachedButtonAdapterData.m_usdHoveredColor.data()[2], m_cachedButtonAdapterData.m_usdHoveredColor.data()[3]);
            data.clickedColor = nau::math::Color4(m_cachedButtonAdapterData.m_usdClickedColor.data()[0], m_cachedButtonAdapterData.m_usdClickedColor.data()[1], m_cachedButtonAdapterData.m_usdClickedColor.data()[2], m_cachedButtonAdapterData.m_usdClickedColor.data()[3]);
            data.disableColor = nau::math::Color4(m_cachedButtonAdapterData.m_usdDisableColor.data()[0], m_cachedButtonAdapterData.m_usdDisableColor.data()[1], m_cachedButtonAdapterData.m_usdDisableColor.data()[2], m_cachedButtonAdapterData.m_usdDisableColor.data()[3]);

            data.defaultScale = m_cachedButtonAdapterData.m_defaultImageScale;
            data.hoveredScale = m_cachedButtonAdapterData.m_hoveredImageScale;
            data.clickedScale = m_cachedButtonAdapterData.m_clickedImageScale;
            data.disableScale = m_cachedButtonAdapterData.m_disableImageScale;

            m_button->updateButtonData(data);
        }
    }

    void UsdUiButtonAdapter::validateButtonDataCache()
    {
        const GuiNauGuiButton usdButton{ getPrim() };

        ButtonAdapterData data;

        usdButton.GetTextureDefaultImageAttr().Get(&data.m_sdfPathDefaultImage);
        usdButton.GetTextureHoveredImageAttr().Get(&data.m_sdfPathHoveredImage);
        usdButton.GetTextureClickedImageAttr().Get(&data.m_sdfPathClickedImage);
        usdButton.GetTextureDisableImageAttr().Get(&data.m_sdfPathDisableImage);   

        usdButton.GetDefaultScaleAttr().Get(&data.m_defaultImageScale);
        usdButton.GetHoveredScaleAttr().Get(&data.m_hoveredImageScale);
        usdButton.GetClickedScaleAttr().Get(&data.m_clickedImageScale);
        usdButton.GetDisableScaleAttr().Get(&data.m_disableImageScale);

        usdButton.GetDefaultColorAttr().Get(&data.m_usdDefaultColor);
        usdButton.GetHoveredColorAttr().Get(&data.m_usdHoveredColor);
        usdButton.GetClickedColorAttr().Get(&data.m_usdClickedColor);
        usdButton.GetDisableColorAttr().Get(&data.m_usdDisableColor);

        bool isFirstInitialization = !m_cachedButtonAdapterData.m_initialized;
        bool isDataChanged = !(m_cachedButtonAdapterData == data);

        m_cachedButtonAdapterData = data;
        m_cachedButtonAdapterData.m_initialized = true;
        m_cachedButtonAdapterData.m_isDirty = isFirstInitialization || isDataChanged;
    }

    nau::ui::NauButton* UsdUiButtonAdapter::createButton()
    {
        const GuiNauGuiButton usdButton{ getPrim() };

        PXR_NS::SdfAssetPath sdfPathDefaultImage;
        PXR_NS::SdfAssetPath sdfPathHoveredImage;
        PXR_NS::SdfAssetPath sdfPathClickedImage;
        PXR_NS::SdfAssetPath sdfPathDisableImage;

        usdButton.GetTextureDefaultImageAttr().Get(&sdfPathDefaultImage);
        usdButton.GetTextureHoveredImageAttr().Get(&sdfPathHoveredImage);
        usdButton.GetTextureClickedImageAttr().Get(&sdfPathClickedImage);
        usdButton.GetTextureDisableImageAttr().Get(&sdfPathDisableImage);

        nau::ui::NauButtonData data;
        data.defaultImageFileName = getSourcePath(sdfPathDefaultImage);
        data.hoveredImageFileName = getSourcePath(sdfPathHoveredImage);
        data.clickedImageFileName = getSourcePath(sdfPathClickedImage);
        data.disableImageFileName = getSourcePath(sdfPathDisableImage);

        double defaultImageScale;
        double hoveredImageScale;
        double clickedImageScale;
        double disableImageScale;

        usdButton.GetDefaultScaleAttr().Get(&defaultImageScale);
        usdButton.GetHoveredScaleAttr().Get(&hoveredImageScale);
        usdButton.GetClickedScaleAttr().Get(&clickedImageScale);
        usdButton.GetDisableScaleAttr().Get(&disableImageScale);

        data.clickedScale = clickedImageScale;
        data.defaultScale = defaultImageScale;
        data.hoveredScale = hoveredImageScale;
        data.disableScale = disableImageScale;

        nau::ui::NauButton* button = nau::ui::NauButton::create(data);
        id = button->getUid();

        return button;
    }

    void UsdUiButtonAdapter::destroyNode()
    {
        m_button->removeFromParent();
        m_button->release();
    }

    DEFINE_UI_TRANSLATOR(UsdUiButtonAdapter, "NauGuiButton"_tftoken);
}
