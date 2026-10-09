// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_prim_adapter.h"
#include <pxr/usd/usd/attribute.h>


namespace UsdTranslator
{

    IPrimAdapter::IPrimAdapter(PXR_NS::UsdPrim prim) :
        m_prim(prim),
        m_path(prim.GetPrimPath())
    {
    }

    IPrimAdapter::~IPrimAdapter()
    {
    }

    PXR_NS::UsdPrim IPrimAdapter::getPrim() const
    {
        return m_prim;
    }

    const std::map<PXR_NS::TfToken, UsdTranslator::IPrimAdapter::Ptr>& IPrimAdapter::getChildren() const
    {
        return m_children;
    }

    IPrimAdapter::Ptr IPrimAdapter::getChild(PXR_NS::TfToken name) const
    {
        auto it = m_children.find(name);
        if (it == m_children.end())
            return nullptr;
        return it->second;
    }

    void IPrimAdapter::addChild(PXR_NS::TfToken name, Ptr adapter)
    {
        m_children[name] = adapter;
    }

    void IPrimAdapter::destroyChild(PXR_NS::TfToken name)
    {
        auto adapter = m_children[name];
        adapter->destroy();
        m_children.erase(name);
    }

    PXR_NS::SdfPath IPrimAdapter::getPrimPath() const
    {
        return m_path;
    }

    void IPrimAdapter::setError(const std::string& error)
    {
        if(!m_prim)
            return;

        m_prim.CreateAttribute(PXR_NS::TfToken("error"), PXR_NS::SdfValueTypeNames->String).Set(error);
    }

    void IPrimAdapter::clearError()
    {
        if (!m_prim)
            return;
        if (m_prim.HasAttribute(PXR_NS::TfToken("error")))
        {
            m_prim.RemoveProperty(PXR_NS::TfToken("error"));
        }
    }

    void IPrimAdapter::destroy()
    {
        for (auto ch : m_children)
            ch.second->destroy();

        destroySceneObject();
    }

}  // namespace UsdTranslator
