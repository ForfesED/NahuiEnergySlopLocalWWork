// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <nau/scene/scene.h>
#include <nau/rtti/rtti_impl.h>
#include <pxr/usd/usd/stage.h>

#include "usd_proxy/usd_proxy.h"
#include "usd_translator/usd_prim_adapter.h"
#include "usd_translator/usd_translator_api.h"

namespace UsdTranslator
{
    class USD_TRANSLATOR_API ComponentAdapter : public IPrimAdapter
    {
    public:
        using Ptr = std::shared_ptr<ComponentAdapter>;

        ComponentAdapter(PXR_NS::UsdPrim prim);
        ~ComponentAdapter() override;

        std::string_view getType() const override;
        nau::async::Task<nau::scene::ObjectWeakRef<nau::scene::SceneObject>> initializeSceneObject(nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest) override;
        nau::scene::ObjectWeakRef<nau::scene::SceneObject> getSceneObject() const override;
        nau::scene::ObjectWeakRef<nau::scene::Component> getComponent() const;
        nau::async::Task<> update() override;
        bool isValid() const override;

    protected:
        virtual void destroySceneObject() override;
        void applyAttributesToComponent();
        nau::rtti::TypeInfo getComponentTypeFromPrim() const;

    protected:
        nau::scene::ObjectWeakRef<nau::scene::SceneObject> m_obj = nullptr;
        nau::scene::ObjectWeakRef<nau::scene::Component> m_component = nullptr;
    };

}  // namespace UsdTranslator
