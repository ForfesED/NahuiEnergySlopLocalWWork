// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <nau/async/task.h>
#include <nau/scene/scene.h>
#include <pxr/usd/usd/stage.h>

#include "usd_proxy/usd_proxy.h"
#include "usd_translator/usd_prim_adapter.h"
#include "usd_translator/usd_translator_api.h"

namespace UsdTranslator
{
    class USD_TRANSLATOR_API StageTranslator final
    {
    public:
        StageTranslator();
        ~StageTranslator();

        StageTranslator& setTarget(nau::scene::IScene::WeakRef scene);
        nau::scene::IScene::WeakRef getTarget() const;
        StageTranslator& setSource(PXR_NS::UsdStagePtr usdStage, const PXR_NS::SdfPath& rootPath = PXR_NS::SdfPath(""));
        PXR_NS::UsdStagePtr getSource() const;
        const PXR_NS::SdfPath& getRootPath() const;
        IPrimAdapter::Ptr getRootAdapter() const;
        nau::async::Task<> forceUpdate(PXR_NS::UsdPrim prim);

        StageTranslator& follow();

        nau::async::Task<> initScene();

    private:
        StageTranslator(const StageTranslator&) = delete;
        StageTranslator(StageTranslator&&) = delete;
        StageTranslator& operator=(const StageTranslator&) = delete;
        StageTranslator& operator=(StageTranslator&&) = delete;

        nau::async::Task<> initSceneObjects(PXR_NS::UsdPrim prim, nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest, IPrimAdapter::Ptr& parent);

        nau::async::Task<> m_watchTask;
        nau::scene::IScene::WeakRef m_scene;
        PXR_NS::UsdStagePtr m_usdStage;
        PXR_NS::SdfPath m_rootPath;
        IPrimAdapter::Ptr m_rootAdapter;
        std::shared_ptr<UsdProxy::StageObjectChangedWatcher> m_watcher;
    };

}  // namespace UsdTranslator
