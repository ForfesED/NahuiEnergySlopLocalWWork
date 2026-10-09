// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "../usd_prim_translator.h"
#include "usd_ui_prim_adapter.h"

#include "usd_ui_node_adapter.h"

namespace nau::ui { class NauLabel; }

namespace UsdTranslator
{
    class UsdUiLabelAdapter : public UsdUiNodeAdapter
    {
        struct LabelAdapterData
        {
            bool m_initialized {false};
            bool m_isDirty {false};

            std::string m_text {""};

            PXR_NS::SdfAssetPath m_sdfFontPath {""};

            int m_overflowType;
            int m_wrappingype;
            int m_horizontalAlignmentType;
            int m_verticalAlignmentType;

            bool operator==(const LabelAdapterData& other) const 
            {
                return m_text == other.m_text &&
                       
                       m_sdfFontPath == other.m_sdfFontPath &&

                       m_overflowType == other.m_overflowType &&
                       m_wrappingype == other.m_wrappingype &&
                       m_horizontalAlignmentType == other.m_horizontalAlignmentType &&
                       m_verticalAlignmentType == other.m_verticalAlignmentType;
            }
        };

    public:
        UsdUiLabelAdapter(PXR_NS::UsdPrim prim);

        bool isValid() const override;
        void update() override;

        nau::Uid getUid() const override;

        virtual nau::ui::Node* initializeNode() override;
        virtual nau::ui::Node* getNode() const override;

        void addChildInternal(nau::ui::Node* node) override;

        virtual std::string getType() const override { return "label"; }
        virtual void serializeNodeContent(nau::DataBlock &blk) override;

    private:
        nau::ui::NauLabel* createLabel();

    protected:
        void internalUpdate() override;
        void destroyNode() override;

    private:
        nau::ui::NauLabel* m_label = nullptr;
        nau::Uid id;
        LabelAdapterData m_cachedAdapterData {};

    private:
        void validateDataCache();
    };
}
