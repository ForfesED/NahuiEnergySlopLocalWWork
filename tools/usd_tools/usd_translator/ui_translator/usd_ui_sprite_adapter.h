// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "../usd_prim_translator.h"
#include "usd_ui_prim_adapter.h"

#include "usd_ui_node_adapter.h"

namespace nau::ui { class Sprite; }

namespace UsdTranslator
{
    class UsdUiSpriteAdapter : public UsdUiNodeAdapter
    {
        struct SpriteAdapterData
        {
            bool m_initialized {false};
            bool m_isDirty {false};

            PXR_NS::SdfAssetPath m_sdfTexturePath {""};

            bool operator==(const SpriteAdapterData& other) const 
            {
                return m_sdfTexturePath == other.m_sdfTexturePath;
            }
        };

    public:
        UsdUiSpriteAdapter(PXR_NS::UsdPrim prim);

        bool isValid() const override;
        void update() override;

        nau::Uid getUid() const override;

        virtual nau::ui::Node* initializeNode() override;
        virtual nau::ui::Node* getNode() const override;

        void addChildInternal(nau::ui::Node* node) override;

        virtual void serializeNodeContent(nau::DataBlock &blk) override;
        virtual std::string getType () const override { return "sprite"; }

    private:
        nau::ui::Sprite* createSprite();

    protected:
        void destroyNode() override;
        void internalUpdate() override;

    private:
        nau::ui::Sprite* m_sprite = nullptr;
        nau::Uid id;
        SpriteAdapterData m_cachedAdapterData {};

    private:
        void validateDataCache();
    };
}
