// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_scene_component_adapter.h"
#include "usd_prim_translator.h"

#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/xformCache.h>

#include "nau/diag/logging.h"
#include "nau/scene/scene_factory.h"
#include "nau/service/service_provider.h"
#include "nau/usd_wrapper/usd_attribute_wrapper.h"
#include "nau/scene/components/internal/missing_component.h"

namespace UsdTranslator
{
    namespace
    {
        constexpr std::string_view g_typeName = "NauSceneComponent";
    }

    SceneComponentAdapter::SceneComponentAdapter(PXR_NS::UsdPrim prim) :
        ComponentAdapter(prim)
    {
    }

    SceneComponentAdapter::~SceneComponentAdapter() = default;

    std::string_view SceneComponentAdapter::getType() const
    {
        return g_typeName;
    }

    nau::async::Task<nau::scene::ObjectWeakRef<nau::scene::SceneObject>> SceneComponentAdapter::initializeSceneObject(nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest)
    {
        nau::rtti::TypeInfo componentTypeInfo = getComponentTypeFromPrim();

        // NOTE: If the kind "Component" is not specified, then we need to create a new object and add it to the parent (dest).
        // the root component of the new object is our component.
        PXR_NS::TfToken kindToken;
        getPrim().GetKind(&kindToken);
        if (kindToken == "Component")
        {
            m_obj = dest;
            m_component = co_await m_obj->addComponentAsync(componentTypeInfo);
        }
        else
        {
            using namespace nau::scene;
            ISceneFactory& sceneFactory = nau::getServiceProvider().get<ISceneFactory>();

            auto newChild = sceneFactory.createSceneObject(&componentTypeInfo);
            if(!newChild || newChild->getRootComponent().is<nau::scene::IMissingComponent>())
            {
                NAU_LOG_ERROR("Failed to create scene object for prim: {}", getPrim().GetPath().GetString());
                setError("Specified component type not registered");
                getPrim().SetKind("InvalidComponent"_tftoken);
                co_return nullptr;
            }
            else
            {
                clearError();
            }
            m_obj = *newChild;

            co_await dest->attachChildAsync(std::move(newChild));

            m_component = m_obj->getRootComponent();
        }

        co_await update();
        co_return m_obj;
    }

    nau::async::Task<> SceneComponentAdapter::update()
    {
        updateSceneComponent(getPrim());
        applyAttributesToComponent();

        return nau::async::makeResolvedTask();
    }

    void SceneComponentAdapter::updateSceneComponent(PXR_NS::UsdPrim prim)
    {
        if (!m_component)
        {
            return;
        }

        auto sceneComponent = m_component->as<nau::scene::SceneComponent*>();
        NAU_ASSERT(sceneComponent);

        PXR_NS::UsdGeomXformCache cache;
        bool resetsXformStack = false;
        PXR_NS::GfMatrix4d usdTransform = cache.GetLocalTransformation(prim, &resetsXformStack);

        // decompose matrix
        auto translation = usdTransform.ExtractTranslation();
        auto scale = PXR_NS::GfVec3d(usdTransform.GetRow3(0).GetLength(), usdTransform.GetRow3(1).GetLength(), usdTransform.GetRow3(2).GetLength());
        usdTransform.Orthonormalize(false);
        auto rotation = usdTransform.ExtractRotationQuat();

        sceneComponent->setScale({(float)scale[0], (float)scale[1], (float)scale[2]});
        sceneComponent->setTranslation({(float)translation[0], (float)translation[1], (float)translation[2]});
        auto rotxyz = rotation.GetImaginary();
        sceneComponent->setRotation({(float)rotxyz[0], (float)rotxyz[1], (float)rotxyz[2], (float)rotation.GetReal()});
    }

    DEFINE_TRANSLATOR(SceneComponentAdapter, "NauSceneComponent"_tftoken);
}  // namespace UsdTranslator
