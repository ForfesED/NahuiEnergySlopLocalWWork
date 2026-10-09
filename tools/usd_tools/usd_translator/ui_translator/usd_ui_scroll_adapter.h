// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "../usd_prim_translator.h"
#include "usd_ui_prim_adapter.h"

#include "usd_ui_node_adapter.h"

namespace nau::ui { class NauScroll; }

namespace UsdTranslator
{
    class UsdUiScrollAdapter : public UsdUiNodeAdapter
    {
    public:
        UsdUiScrollAdapter(PXR_NS::UsdPrim prim);

        bool isValid() const override;
        void update() override;

        nau::Uid getUid() const override;

        virtual nau::ui::Node* initializeNode() override;
        virtual nau::ui::Node* getNode() const override;

        void addChildInternal(nau::ui::Node* node) override;
        virtual std::string getType() const override { return "scroll"; }

    private:
        nau::ui::NauScroll* createScroll();

    protected:
        void destroyNode() override;
        void internalUpdate() override;
        void internalPrimUpdate(nau::ui::Node* node) override;

    private:
        nau::ui::NauScroll* m_scroll = nullptr;
        nau::Uid id;
    };
}
