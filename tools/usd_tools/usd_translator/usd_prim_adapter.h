// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <nau/async/task.h>
#include <nau/scene/scene.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>

#include <map>

#include "usd_translator/usd_translator_api.h"

namespace UsdTranslator
{
    class USD_TRANSLATOR_API IPrimAdapter
    {
    public:
        using Ptr = std::shared_ptr<IPrimAdapter>;

        IPrimAdapter(PXR_NS::UsdPrim prim);
        virtual ~IPrimAdapter();

        PXR_NS::UsdPrim getPrim() const;

        const std::map<PXR_NS::TfToken, Ptr>& getChildren() const;
        Ptr getChild(PXR_NS::TfToken name) const;
        void addChild(PXR_NS::TfToken name, Ptr adapter);
        void destroyChild(PXR_NS::TfToken name);
        PXR_NS::SdfPath getPrimPath() const;

        virtual std::string_view getType() const = 0;
        // initialize SceneObject if need return new child
        virtual nau::async::Task<nau::scene::ObjectWeakRef<nau::scene::SceneObject>> initializeSceneObject(nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest) = 0;
        virtual nau::scene::ObjectWeakRef<nau::scene::SceneObject> getSceneObject() const = 0;

        virtual nau::async::Task<> update() = 0;
        virtual bool isValid() const = 0;
        /*virtual void release() = 0;
        virtual void setActive(bool val) = 0;
        virtual void isActive() const = 0;*/
    protected:
        virtual void destroySceneObject() = 0;
        virtual void setError(const std::string& error);
        virtual void clearError();
    private:
        void destroy();

        PXR_NS::SdfPath m_path;
        std::map<PXR_NS::TfToken, Ptr> m_children;
        PXR_NS::UsdPrim m_prim;
    };
}  // namespace UsdTranslator
