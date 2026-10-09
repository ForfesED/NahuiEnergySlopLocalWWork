// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "../usd_prim_translator.h"
#include "usd_ui_prim_adapter.h"

#include "usd_ui_node_adapter.h"

namespace nau::ui { class NauButton; }

namespace UsdTranslator
{
    class UsdUiButtonAdapter : public UsdUiNodeAdapter
    {
        struct ButtonAdapterData
        {
            bool m_initialized {false};
            bool m_isDirty {false};

            PXR_NS::SdfAssetPath m_sdfPathDefaultImage {""};
            PXR_NS::SdfAssetPath m_sdfPathHoveredImage {""};
            PXR_NS::SdfAssetPath m_sdfPathClickedImage {""};
            PXR_NS::SdfAssetPath m_sdfPathDisableImage {""};

            double m_defaultImageScale {0.0};
            double m_hoveredImageScale {0.0};
            double m_clickedImageScale {0.0};
            double m_disableImageScale {0.0};

            pxr::GfVec4d m_usdDefaultColor {};
            pxr::GfVec4d m_usdHoveredColor {};
            pxr::GfVec4d m_usdClickedColor {};
            pxr::GfVec4d m_usdDisableColor {};

            bool operator==(const ButtonAdapterData& other) const 
            {
                return m_sdfPathDefaultImage == other.m_sdfPathDefaultImage &&
                       m_sdfPathHoveredImage == other.m_sdfPathHoveredImage &&
                       m_sdfPathClickedImage == other.m_sdfPathClickedImage &&
                       m_sdfPathDisableImage == other.m_sdfPathDisableImage && 

                       m_defaultImageScale == other.m_defaultImageScale &&
                       m_hoveredImageScale == other.m_hoveredImageScale &&
                       m_clickedImageScale == other.m_clickedImageScale &&
                       m_disableImageScale == other.m_disableImageScale &&

                       m_usdDefaultColor == other.m_usdDefaultColor &&
                       m_usdHoveredColor == other.m_usdHoveredColor &&
                       m_usdClickedColor == other.m_usdClickedColor &&
                       m_usdDisableColor == other.m_usdDisableColor;
            }
        };

    public:
        UsdUiButtonAdapter(PXR_NS::UsdPrim prim);

        bool isValid() const override;
        void update() override;

        nau::Uid getUid() const override;

        virtual nau::ui::Node* initializeNode() override;
        virtual nau::ui::Node* getNode() const override;

        void addChildInternal(nau::ui::Node* node) override;

        virtual void serializeNodeContent(nau::DataBlock &blk) override;
        virtual std::string getType() const override { return "button"; }

    private:
        nau::ui::NauButton* createButton();

    protected:
        void destroyNode() override;
        void internalUpdate() override;

    private:
        nau::ui::NauButton* m_button = nullptr;
        nau::Uid id;
        ButtonAdapterData m_cachedButtonAdapterData {};

    private:
        void validateButtonDataCache();
    };
}
