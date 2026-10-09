// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "usd_translator/usd_translator_api.h"
#include "usd_translator/usd_prim_adapter.h"
#include "usd_proxy/usd_proxy.h"

#include <nau/scene/scene.h>


namespace UsdTranslator
{
    class USD_TRANSLATOR_API AudioEmitterAdapter : public IPrimAdapter
    {
    public:
        AudioEmitterAdapter(pxr::UsdPrim prim);

        std::string_view getType() const override;
        nau::async::Task<nau::scene::ObjectWeakRef<nau::scene::SceneObject>> initializeSceneObject(nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest) override;
        nau::scene::ObjectWeakRef<nau::scene::SceneObject> getSceneObject() const override;
        nau::async::Task<> update() override;
        bool isValid() const override;

    protected:
        void destroySceneObject() override;

    private:
        nau::scene::ObjectWeakRef<nau::scene::SceneObject> m_obj = nullptr;
    };
}