// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_mesh_adapter.h"

#include <pxr/base/gf/matrix2d.h>
#include <pxr/base/gf/matrix2f.h>
#include <pxr/base/gf/matrix3d.h>
#include <pxr/base/gf/matrix3f.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/matrix4f.h>
#include <pxr/base/gf/vec2d.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/base/gf/vec2i.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/gf/vec3i.h>
#include <pxr/base/gf/vec4d.h>
#include <pxr/base/gf/vec4f.h>
#include <pxr/base/gf/vec4i.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>
#include <pxr/usd/usdSkel/bindingAPI.h>

#include <fstream>

#include "nau/animation/components/skeleton_component.h"
#include "nau/asset_tools/asset_info.h"
#include "nau/asset_tools/db_manager.h"
#include "nau/assets/asset_container.h"
#include "nau/assets/asset_descriptor_factory.h"
#include "nau/assets/asset_manager.h"
#include "nau/assets/material.h"
#include "nau/assets/mesh_asset_accessor.h"
#include "nau/io/file_system.h"
#include "nau/io/virtual_file_system.h"
#include "nau/platform/windows/utils/uid.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/scene/components/skinned_mesh_component.h"
#include "nau/scene/scene_factory.h"
#include "nau/serialization/json_utils.h"
#include "nau/service/service_provider.h"
#include "nau/usd_meta_tools/usd_meta_manager.h"
#include "usd_prim_translator.h"
#include "usd_mesh_container.h"

using namespace nau;
using namespace nau::scene;
using namespace PXR_NS;
namespace fs = std::filesystem;

namespace UsdTranslator
{
    namespace
    {
        constexpr std::string_view g_typeName = "Mesh";
        TfToken g_materialAssign = "Material:assign"_tftoken;

        nau::Result<AssetMetaInfo> getAssetInfo(Uid uid)
        {
            auto& dbManager = AssetDatabaseManager::instance();
            auto& vfs = getServiceProvider().get<io::IVirtualFileSystem>();
            auto projectPath = fs::path(vfs.resolveToNativePath("/content")).parent_path();
            const fs::path assetsDb = fs::path(projectPath) / getAssetsDBfolderName();

            if (!dbManager.isLoaded())
            {
                const fs::path dbFilePath = assetsDb / getAssetsDbName();
                NAU_VERIFY(dbManager.load(assetsDb.string()), "Failed to load assets database!");
                auto contentFs = io::createNativeFileSystem(assetsDb.string());
                vfs.mount(getAssetsDBfolderName(), contentFs).ignore();
            }
            return dbManager.get(uid);
        }

        class MeshMaterialAssignDecorator : public UsdProxy::IUsdProxyPrimDecorator
        {
        public:
            void decorate(UsdProxy::ProxyPrimContextPtr context) override
            {
                auto primType = context->getPrim().GetTypeName();
                if (primType == "Mesh")
                {
                    auto prop = std::make_shared<UsdProxy::ProxyPropertyContext>();
                    prop->setDefaultValue(PXR_NS::VtValue(SdfAssetPath()))
                        .setName(PXR_NS::TfToken(g_materialAssign))
                        .setType(PXR_NS::SdfSpecType::SdfSpecTypeAttribute);
                    context->tryInsertProperty(prop);
                }
            }
        };

        REGISTRY_PROXY_DECORATOR(MeshMaterialAssignDecorator);
    }  // namespace


    MeshAdapter::MeshAdapter(PXR_NS::UsdPrim prim) :
        IPrimAdapter(prim),
        m_container(rtti::createInstance<UsdMeshContainer, IAssetContainer>(prim))
    {
        const AssetPath assetPath{"usd", strings::toStringView(prim.GetPath().GetAsString())};
        getServiceProvider().get<IAssetDescriptorFactory>().addAssetContainer(assetPath, m_container);
    }

    MeshAdapter::~MeshAdapter()
    {
        const AssetPath assetPath{"usd", strings::toStringView(getPrim().GetPath().GetAsString())};
        getServiceProvider().get<IAssetDescriptorFactory>().removeAssetContainer(assetPath);
    }

    std::string_view MeshAdapter::getType() const
    {
        return g_typeName;
    }

    async::Task<ObjectWeakRef<SceneObject>> MeshAdapter::initializeSceneObject(ObjectWeakRef<SceneObject> dest)
    {
        ISceneFactory& sceneFactory = getServiceProvider().get<ISceneFactory>();
        IAssetDescriptorFactory& assetManager = getServiceProvider().get<IAssetDescriptorFactory>();

        ObjectUniquePtr<SceneObject> newChild;
        pxr::UsdSkelBindingAPI bindingApi{getPrim()};
        if (const UsdGeomPrimvar& primvar = bindingApi.GetJointIndicesPrimvar())  // is skinned
        {
            newChild = sceneFactory.createSceneObject<SkinnedMeshComponent>();
            m_obj = *newChild;

            IAssetDescriptor::Ptr meshAsset = assetManager.createAssetDescriptor(*m_container, {});
            SkinnedMeshComponent& meshComponent = m_obj->getRootComponent<SkinnedMeshComponent>();
            co_await m_obj->addComponentAsync<nau::SkeletonComponent>();
            // TODO: Delete after adding materials support
            meshComponent.setMaterial(AssetPath{ "file:/content/materials/embedded/standard_skinned.nmat_json" });
            meshComponent.setMeshGeometry(std::move(meshAsset));
        }
        else
        {
            newChild = sceneFactory.createSceneObject<StaticMeshComponent>();
            m_obj = *newChild;

            IAssetDescriptor::Ptr meshAsset = assetManager.createAssetDescriptor(*m_container, {});
            StaticMeshComponent& meshComponent = m_obj->getRootComponent<StaticMeshComponent>();
            meshComponent.setMeshGeometry(std::move(meshAsset));

            if (VtValue materialPath; UsdProxy::UsdProxyPrim(getPrim()).getProperty(g_materialAssign)->getValue(&materialPath))
            {
                if (materialPath.CanCast<SdfAssetPath>())
                {
                    auto assetPath = materialPath.Get<SdfAssetPath>().GetResolvedPath();
                    assignMaterial(assetPath, meshComponent);
                }
            }
        }

        const auto name = getPrim().GetName().GetString();
        m_obj->setName({name.data(), name.length()});

        co_await update();

        co_return co_await dest->attachChildAsync(std::move(newChild));
    }

    void MeshAdapter::assignMaterial(const std::string& assetPath, nau::scene::StaticMeshComponent& meshComponent)
    {
        using namespace nau::scene;

        if (assetPath.empty() || !fs::exists(assetPath))
            return;

        auto info = UsdMetaManager::instance().getInfo(assetPath);
        auto uidStr = toString(info[0].uid);
        auto assetInfo = getAssetInfo(info[0].uid);

        m_materialPath = assetPath;
        m_coreMaterialPath = "uid:" + uidStr;
        m_materialTimeStamp = std::format("{}", fs::last_write_time(assetPath));

        MaterialAssetRef matDefault{strings::toStringView(m_coreMaterialPath)};
        meshComponent.setMaterial(matDefault);
    }

    nau::async::Task<> MeshAdapter::update()
    {
        using namespace nau::scene;
        translateWorldTransform(getPrim(), *m_obj);

        if (VtValue materialPath; UsdProxy::UsdProxyPrim(getPrim()).getProperty(g_materialAssign)->getValue(&materialPath))
        {
            if (materialPath.CanCast<SdfAssetPath>())
            {
                auto assetPath = materialPath.Get<SdfAssetPath>().GetResolvedPath();
                if (assetPath.empty() || !fs::exists(assetPath))
                    return nau::async::makeResolvedTask();

                if (m_materialPath == assetPath && m_materialTimeStamp == std::format("{}", fs::last_write_time(assetPath)))
                    return nau::async::makeResolvedTask();

                if (!m_coreMaterialPath.empty())
                {
                    getServiceProvider().get<IAssetManager>().removeAsset(strings::toStringView(m_coreMaterialPath));
                }

                if (StaticMeshComponent* meshComponent = m_obj->findFirstComponent<StaticMeshComponent>())
                    assignMaterial(assetPath, *meshComponent);
            }
        }

        return nau::async::makeResolvedTask();
    }

    bool MeshAdapter::isValid() const
    {
        return !!m_obj;
    }

    void MeshAdapter::destroySceneObject()
    {
        m_obj->destroy();
        m_obj = nullptr;
    }

    nau::scene::ObjectWeakRef<nau::scene::SceneObject> MeshAdapter::getSceneObject() const
    {
        return m_obj;
    }

    DEFINE_TRANSLATOR(MeshAdapter, "Mesh"_tftoken);
}  // namespace UsdTranslator