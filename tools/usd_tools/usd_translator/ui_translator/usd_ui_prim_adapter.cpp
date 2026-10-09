// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_prim_adapter.h"
#include "usd_proxy/usd_proxy.h"

#include "nau/assets/asset_db.h"
#include "nau/service/service_provider.h"

#include <filesystem>

namespace  UsdTranslator
{

    IUIPrimAdapter::IUIPrimAdapter(PXR_NS::UsdPrim prim)
        : m_prim(prim), m_path(prim.GetPrimPath())
    {}

    IUIPrimAdapter::~IUIPrimAdapter()
    {}

    PXR_NS::UsdPrim IUIPrimAdapter::getPrim() const
    {
        return m_prim;
    }

    const std::map<PXR_NS::TfToken, UsdTranslator::IUIPrimAdapter::Ptr>& IUIPrimAdapter::getChildren() const
    {
        return m_children;
    }

    IUIPrimAdapter::Ptr IUIPrimAdapter::getChild(PXR_NS::TfToken name) const
    {
        auto it = m_children.find(name);
        return it == m_children.end() ? nullptr :it->second;
    }

    void IUIPrimAdapter::addChild(PXR_NS::TfToken name, Ptr adapter)
    {
        m_children[name] = adapter;
    }

    void IUIPrimAdapter::destroyChild(PXR_NS::TfToken name)
    {
        auto adapter = m_children[name];
        adapter->destroy();
        m_children.erase(name);
    }

    void IUIPrimAdapter::serializeToBlk(nau::DataBlock& blk)
    {
        for (auto [name, adapter] : getChildren())
        {
            if (adapter)
            {
                nau::DataBlock* elementBlk = blk.addNewBlock("element");
                adapter->toBlk(*elementBlk);
            }
        }
    }

    void IUIPrimAdapter::toBlk(nau::DataBlock &blk)
    {
        serializeNodeContent(blk);
        serializeChildren(blk);
    }

    void IUIPrimAdapter::serializeNodeContent(nau::DataBlock& blk)
    {
    }

    void IUIPrimAdapter::serializeChildren(nau::DataBlock& blk)
    {
        for (auto& [name, adapter] : getChildren())
        {
            if (!adapter)
            {
                continue;
            }

            nau::DataBlock* childrenBlk = blk.addBlock("children");
            nau::DataBlock* childBlock = childrenBlk->addNewBlock("element");
            adapter->toBlk(*childBlock);
        }
    }

    PXR_NS::SdfPath IUIPrimAdapter::getPrimPath() const
    {
        return m_path;
    }

    const std::string IUIPrimAdapter::getSourcePath(const PXR_NS::SdfAssetPath& sdfPath)
    {
        const auto rootLayer = getPrim().GetStage()->GetRootLayer();

        const auto assetPath = sdfPath.GetAssetPath();
        const auto assetAbstPath = rootLayer->ComputeAbsolutePath(assetPath);
        const auto sourcePath = IUIPrimAdapter::sourcePathFromAssetFile(assetAbstPath);

        if (sourcePath.empty())
        {
            NAU_LOG_ERROR("UI Translator: failed to fetch a source path from this asset '{}'", assetPath);
            return "";
        }

        return sourcePath;
    }

    std::string IUIPrimAdapter::sourcePathFromAssetFile(const std::string& assetPath)
    {
        // TODO: Refactor source getting. Get it from UsdMetaInfo
        auto stage = pxr::UsdStage::Open(assetPath);
        if (!stage)
        {
            return std::string();
        }

        auto rootPrim = stage->GetPrimAtPath(pxr::SdfPath("/Root"));
        if (!rootPrim)
        {
            return std::string();
        }

        auto proxyPrim = UsdProxy::UsdProxyPrim(rootPrim);
        auto uidProperty = proxyPrim.getProperty(pxr::TfToken("uid"));

        if (uidProperty && uidProperty->isAuthored())
        {
            pxr::VtValue val;
            uidProperty->getValue(&val);

            if (val.IsHolding<std::string>())
            {
                auto uid = nau::Uid::parseString(val.Get<std::string>());
                auto& assetDb = nau::getServiceProvider().get<nau::IAssetDB>();
                const auto& assetMetaInfo = assetDb.findAssetMetaInfoByUid(*uid);
                return ("/res/" + assetMetaInfo.sourcePath + "." + assetMetaInfo.sourceType).c_str();
            }
        }

        return std::string();
    }

    void IUIPrimAdapter::destroy()
    {
        for (auto ch : m_children)
            ch.second->destroy();

        destroyNode();
    }

}
