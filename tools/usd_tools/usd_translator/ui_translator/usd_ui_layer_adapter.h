// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "../usd_prim_translator.h"
#include "usd_ui_prim_adapter.h"

#include "usd_ui_node_adapter.h"

namespace nau::ui { class Layer; }

namespace UsdTranslator
{
    class UsdUiLayerAdapter : public UsdUiNodeAdapter
    {
    public:
        UsdUiLayerAdapter(PXR_NS::UsdPrim prim);

        bool isValid() const override;
        void update() override;

        nau::Uid getUid() const override;

        virtual nau::ui::Node* initializeNode() override;
        virtual nau::ui::Node* getNode() const override;

        void addChildInternal(nau::ui::Node* node) override;

        virtual std::string getType() const override { return "layer"; }

    private:
        nau::ui::Layer* createLayer();

    protected:
        void destroyNode() override;
        void internalUpdate() override;

    private:
        nau::ui::Layer* m_layer = nullptr;
        nau::Uid id;
    };
}
