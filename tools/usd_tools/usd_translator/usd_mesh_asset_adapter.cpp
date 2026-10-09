// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_mesh_asset_adapter.h"

#include <pxr/usd/usdGeom/mesh.h>

#include "nau/asset_tools/db_manager.h"
#include "nau/assets/asset_manager.h"
#include "nau/assets/asset_ref.h"
#include "nau/assets/scene_asset.h"
#include "nau/io/virtual_file_system.h"
#include "nau/scene/scene_factory.h"
#include "nau/service/service_provider.h"
#include "nau/usd_meta_tools/usd_meta_manager.h"
#include "usd_prim_translator.h"

using namespace PXR_NS;
using namespace nau;
namespace fs = std::filesystem;

namespace UsdTranslator
{
    namespace
    {
        constexpr std::string_view g_typeName = "NauAssetMesh";
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
                vfs.mount(getAssetsDBfolderName(), contentFs);
            }
            return dbManager.get(uid);
        }

        class MeshAssetMaterialAssignDecorator : public UsdProxy::IUsdProxyPrimDecorator
        {
        public:
            void decorate(UsdProxy::ProxyPrimContextPtr context) override
            {
                auto prim = context->getPrim();
                auto primType = prim.GetTypeName();
                if (primType != "NauAssetMesh")
                    return;

                auto setVisibility = [&](TfToken name, bool val)
                {
                    auto errorProp = std::make_shared<UsdProxy::ProxyPropertyContext>();
                    errorProp->setName(name)
                        .setMetadata("visible"_tftoken, VtValue(val));
                    context->tryInsertProperty(errorProp);
                };

                setVisibility("uid"_tftoken, false);
                setVisibility("meshSource"_tftoken, false);
                setVisibility("ignoreAnimation"_tftoken, false);
                setVisibility("unitScale"_tftoken, false);
                setVisibility("upAxis"_tftoken, false);
                setVisibility("generateLods"_tftoken, false);
                setVisibility("generateCollider"_tftoken, false);
                setVisibility("generateTangents"_tftoken, false);
                setVisibility("skinned"_tftoken, false);
                setVisibility("skeleton"_tftoken, false);
                setVisibility("flipU"_tftoken, false);
                setVisibility("flipV"_tftoken, false);
                setVisibility("path"_tftoken, false);

                auto prop = std::make_shared<UsdProxy::ProxyPropertyContext>();
                prop->setName(g_materialAssign)
                    .setDefaultValue(VtValue(SdfAssetPath()))
                    .setType(SdfSpecType::SdfSpecTypeAttribute);
                context->tryInsertProperty(prop);

                auto setError = [&](const std::string& error)
                {
                    auto errorProp = std::make_shared<UsdProxy::ProxyPropertyContext>();
                    errorProp->setDefaultValue(VtValue(error))
                        .setName("error"_tftoken)
                        .setMetadata("debug"_tftoken, VtValue(true))
                        .setType(SdfSpecType::SdfSpecTypeAttribute);
                    context->tryInsertProperty(errorProp);
                };

                std::string uidStr;
                if (!prim.GetAttribute("uid"_tftoken).Get(&uidStr))
                {
                    setError("Failed to get uid attribute " + uidStr + " " + prim.GetPath().GetString());
                    return;
                }
                auto uid = Uid::parseString(uidStr.c_str());
                if (!uid)
                {
                    setError("Failed to parse uid attribute " + uidStr + " " + prim.GetPath().GetString());
                    return;
                }

                auto assetInfo = getAssetInfo(*uid);
                if (!assetInfo)
                {
                    setError("Failed to get asset info " + uidStr + " " + prim.GetPath().GetString());
                    return;
                }
                auto asset = *assetInfo;
                auto meshRef = std::string(getAssetsDBfolderName()) + "/" + asset.dbPath.c_str();
                std::replace(meshRef.begin(), meshRef.end(), '\\', '/');

                auto& vfs = getServiceProvider().get<io::IVirtualFileSystem>();
                if (!vfs.exists(meshRef))
                {
                    setError(std::format("Asset not found {}", meshRef));
                    return;
                }
            }
        };
        REGISTRY_PROXY_DECORATOR(MeshAssetMaterialAssignDecorator);
    }  // namespace

    AssetMeshAdapter::AssetMeshAdapter(UsdPrim prim) :
        IPrimAdapter(prim)
    {
    }

    AssetMeshAdapter::~AssetMeshAdapter()
    {
    }

    std::string_view AssetMeshAdapter::getType() const
    {
        return g_typeName;
    }

    async::Task<scene::ObjectWeakRef<scene::SceneObject>> AssetMeshAdapter::initializeSceneObject(scene::ObjectWeakRef<scene::SceneObject> dest)
    {
        auto prim = getPrim();
        if (!prim)
            co_return nullptr;

        std::string uidStr;
        if (!prim.GetAttribute("uid"_tftoken).Get(&uidStr))
            co_return nullptr;
        auto uid = Uid::parseString(uidStr.c_str());
        if (!uid)
            co_return nullptr;

        auto assetInfo = getAssetInfo(*uid);
        if (!assetInfo)
            co_return nullptr;

        auto asset = *assetInfo;
        auto meshRef = std::string(getAssetsDBfolderName()) + "/" + asset.dbPath.c_str();
        std::replace(meshRef.begin(), meshRef.end(), '\\', '/');

        auto& vfs = getServiceProvider().get<io::IVirtualFileSystem>();
        if (!vfs.exists(meshRef))
            co_return nullptr;

        bool isSkinned = false;
        prim.GetAttribute("skinned"_tftoken).Get(&isSkinned);
        auto& sceneFactory = getServiceProvider().get<scene::ISceneFactory>();

        auto createComponent = [&]<typename T>()
        {
            auto component = sceneFactory.createSceneObject<T>();
            component->setName(prim.GetName().GetText());
            auto& meshComponent = component->getRootComponent<T>();
            AssetPath assetPath{"uid", strings::toStringView(uidStr), "mesh/0"};
            meshComponent.setMeshGeometry(assetPath);

            if (VtValue materialPath; UsdProxy::UsdProxyPrim(getPrim()).getProperty(g_materialAssign)->getValue(&materialPath))
            {
                if (materialPath.CanCast<SdfAssetPath>())
                {
                    auto assetPath = materialPath.Get<SdfAssetPath>().GetResolvedPath();
                    assignMaterial(assetPath, meshComponent);
                }
            }

            return component;
        };

        auto newChild = isSkinned
                            ? createComponent.operator()<SkinnedMeshComponent>()
                            : createComponent.operator()<scene::StaticMeshComponent>();

        m_obj = *newChild;
        co_await update();
        co_return co_await dest->attachChildAsync(std::move(newChild));
    }

    template <class T>
    void AssetMeshAdapter::assignMaterial(const std::string& assetPath, T& meshComponent)
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

    async::Task<> AssetMeshAdapter::update()
    {
        using namespace nau::scene;
        if(!m_obj) 
            return nau::async::makeResolvedTask();

        translateWorldTransform(getPrim(), *m_obj);

        auto matAssignProp = UsdProxy::UsdProxyPrim(getPrim()).getProperty(g_materialAssign);
        if (VtValue materialPath; matAssignProp && matAssignProp->getValue(&materialPath))
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
                else if (SkinnedMeshComponent* meshComponent = m_obj->findFirstComponent<SkinnedMeshComponent>())
                    assignMaterial(assetPath, *meshComponent);
            }
        }

        return nau::async::makeResolvedTask();
    }

    scene::ObjectWeakRef<scene::SceneObject> AssetMeshAdapter::getSceneObject() const
    {
        return m_obj;
    }

    bool AssetMeshAdapter::isValid() const
    {
        return !!m_obj;
    }

    void AssetMeshAdapter::destroySceneObject()
    {
        if(m_obj)
            m_obj->destroy();
        m_obj = nullptr;
    }

    DEFINE_TRANSLATOR(AssetMeshAdapter, "NauAssetMesh"_tftoken);
}  // namespace UsdTranslator
