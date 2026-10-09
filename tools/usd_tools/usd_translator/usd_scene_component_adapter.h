// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <nau/scene/scene.h>

#include "usd_translator/usd_component_adapter.h"

namespace UsdTranslator
{
    class USD_TRANSLATOR_API SceneComponentAdapter : public ComponentAdapter
    {
    public:
        using Ptr = std::shared_ptr<SceneComponentAdapter>;

        SceneComponentAdapter(PXR_NS::UsdPrim prim);
        ~SceneComponentAdapter() override;

        std::string_view getType() const override;
        nau::async::Task<nau::scene::ObjectWeakRef<nau::scene::SceneObject>> initializeSceneObject(nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest) override;
        nau::async::Task<> update() override;

    private:
        void updateSceneComponent(PXR_NS::UsdPrim prim);
    };

}  // namespace UsdTranslator
