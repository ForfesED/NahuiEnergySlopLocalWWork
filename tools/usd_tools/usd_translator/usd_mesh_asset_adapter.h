// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <nau/scene/scene.h>
#include <pxr/usd/usd/stage.h>

#include "nau/scene/components/skinned_mesh_component.h"
#include "nau/scene/components/static_mesh_component.h"
#include "usd_proxy/usd_proxy.h"
#include "usd_translator/usd_mesh_composer.h"
#include "usd_translator/usd_prim_adapter.h"
#include "usd_translator/usd_translator_api.h"

namespace UsdTranslator
{
    class USD_TRANSLATOR_API AssetMeshAdapter : public IPrimAdapter
    {
    public:
        AssetMeshAdapter(PXR_NS::UsdPrim prim);
        ~AssetMeshAdapter();

        std::string_view getType() const override;
        nau::async::Task<nau::scene::ObjectWeakRef<nau::scene::SceneObject>> initializeSceneObject(nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest) override;
        nau::scene::ObjectWeakRef<nau::scene::SceneObject> getSceneObject() const override;
        nau::async::Task<> update() override;
        bool isValid() const override;

    protected:
        void destroySceneObject() override;

    private:
        nau::scene::ObjectWeakRef<nau::scene::SceneObject> m_obj = nullptr;
        std::string m_materialPath;
        std::string m_coreMaterialPath;
        std::string m_materialTimeStamp;
        template <class T>
        void assignMaterial(const std::string& assetPath, T& meshComponent);
    };
}  // namespace UsdTranslator
