// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "usd_translator/usd_translator_api.h"
#include "usd_translator/ui_translator/usd_ui_prim_adapter.h"
#include "usd_proxy/usd_proxy.h"

#include <pxr/usd/usd/stage.h>

#include "nau/ui.h"
#include "nau/ui/elements/canvas.h"

namespace UsdTranslator
{
    class USD_TRANSLATOR_API UITranslator final
    {
    public:
        UITranslator() = default;
        ~UITranslator() = default;

        UITranslator& setTarget(nau::ui::Canvas* scene);
        nau::ui::Canvas* getTarget() const;
        UITranslator& setSource(PXR_NS::UsdStagePtr usdStage, const PXR_NS::SdfPath& rootPath = PXR_NS::SdfPath::EmptyPath());
        PXR_NS::UsdStagePtr getSource() const;
        const PXR_NS::SdfPath& getRootPath() const;
        IUIPrimAdapter::Ptr getRootAdapter() const;

        UITranslator& follow();

        void initScene();

        // Build scene from USD without instancing ui elements in scene
        void initSceneDataOnly();

    private:
        UITranslator(const UITranslator&) = delete;
        UITranslator(UITranslator&&) = delete;
        UITranslator& operator=(const UITranslator&) = delete;
        UITranslator& operator=(UITranslator&&) = delete;

        PXR_NS::UsdPrim getSceneRoot();
        void initSceneObjects(PXR_NS::UsdPrim prim, nau::ui::Node* sceneRoot, IUIPrimAdapter::Ptr& parent);
        // same as initSceneObjects, but without creating UI elements
        void loadSceneTree(PXR_NS::UsdPrim prim, IUIPrimAdapter::Ptr& parent);

        nau::ui::Canvas* m_scene = nullptr;
        PXR_NS::UsdStagePtr m_usdStage;
        PXR_NS::SdfPath m_rootPath;
        IUIPrimAdapter::Ptr m_rootAdapter;
        std::shared_ptr<UsdProxy::StageObjectChangedWatcher> m_watcher;
    };
}
