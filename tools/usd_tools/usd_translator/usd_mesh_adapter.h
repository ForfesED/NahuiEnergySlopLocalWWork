// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <nau/scene/scene.h>
#include <pxr/usd/usd/stage.h>

#include "usd_proxy/usd_proxy.h"
#include "usd_translator/usd_mesh_composer.h"
#include "usd_translator/usd_prim_adapter.h"
#include "usd_translator/usd_translator_api.h"
#include "nau/scene/components/static_mesh_component.h"

namespace UsdTranslator
{
    class USD_TRANSLATOR_API MeshAdapter : public IPrimAdapter
    {
    public:
        MeshAdapter(PXR_NS::UsdPrim prim);
        ~MeshAdapter();

        std::string_view getType() const override;
        nau::async::Task<nau::scene::ObjectWeakRef<nau::scene::SceneObject>> initializeSceneObject(nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest) override;


        nau::scene::ObjectWeakRef<nau::scene::SceneObject> getSceneObject() const override;
        nau::async::Task<> update() override;
        bool isValid() const override;

    protected:
        virtual void destroySceneObject() override;

    private:
        nau::scene::ObjectWeakRef<nau::scene::SceneObject> m_obj = nullptr;
        nau::Ptr<class UsdMeshContainer> m_container;
        std::string m_materialPath;
        std::string m_coreMaterialPath;
        std::string m_materialTimeStamp;

        void assignMaterial(const std::string& assetPath, nau::scene::StaticMeshComponent& meshComponent);
    };

}  // namespace UsdTranslator
