// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_proxy_decorators_regestry.h"

#include <pxr/usd/sdf/schema.h>
#include <pxr/usd/usd/attribute.h>

namespace UsdProxy
{
    ProxyDecoratorsRegestry& ProxyDecoratorsRegestry::instance()
    {
        static ProxyDecoratorsRegestry inst;
        return inst;
    }

    bool ProxyDecoratorsRegestry::addPrimDecorator(IUsdProxyPrimDecoratorPtr decorator)
    {
        std::lock_guard lock(m_sync);
        auto val = std::find(m_decorators.begin(), m_decorators.end(), decorator);
        if (val != m_decorators.end())
            return false;

        m_decorators.push_back(decorator);
        return true;
    }

    void ProxyDecoratorsRegestry::decorate(ProxyPrimContextPtr context)
    {
        std::lock_guard lock(m_sync);
        for (auto& dec : m_decorators)
            dec->decorate(context);
    }

    ProxyPrimContext::ProxyPrimContext(PXR_NS::UsdPrim prim) :
        m_prim(prim)
    {
    }

    bool ProxyPrimContext::tryInsertProperty(ProxyPropertyContextPtr prop)
    {
        auto& propDest = m_extraProperties[prop->m_name];
        if (propDest)
            return false;
        propDest = prop;
        propDest->m_prim = m_prim;
        return true;
    }

    bool ProxyPrimContext::tryInsertMetadata(const PXR_NS::TfToken& name, const PXR_NS::VtValue& value)
    {
        if (m_metadata.find(name) != m_metadata.end())
            return false;
        m_metadata[name] = value;
        return true;
    }

    bool ProxyPrimContext::tryInsertMetadata(const std::string& name, const PXR_NS::VtValue& value)
    {
        return tryInsertMetadata(PXR_NS::TfToken(name), value);
    }

    PXR_NS::UsdPrim ProxyPrimContext::getPrim() const
    {
        return m_prim;
    }

    ProxyPropertyContext::ProxyPropertyContext(PXR_NS::SdfSpecType type, const PXR_NS::TfToken& name, const PXR_NS::VtValue& defaultValue, const PXR_NS::UsdMetadataValueMap& metadata) :
        m_type(type),
        m_name(name),
        m_metadata(metadata)
    {
        m_metadata[PXR_NS::SdfFieldKeys->Default] = defaultValue;
    }

    ProxyPropertyContext& ProxyPropertyContext::setName(const PXR_NS::TfToken& name)
    {
        m_name = name;
        return *this;
    }

    ProxyPropertyContext& ProxyPropertyContext::setType(PXR_NS::SdfSpecType type)
    {
        m_type = type;
        return *this;
    }

    ProxyPropertyContext& ProxyPropertyContext::setDefaultValue(const PXR_NS::VtValue& defaultValue)
    {
        m_metadata[PXR_NS::SdfFieldKeys->Default] = defaultValue;
        return *this;
    }

    ProxyPropertyContext& ProxyPropertyContext::setMetadata(const PXR_NS::TfToken& key, const PXR_NS::VtValue& value)
    {
        m_metadata[key] = value;
        return *this;
    }
}  // namespace UsdProxy

PXR_NS::TfToken operator""_tftoken(const char* str, size_t val)
{
    return PXR_NS::TfToken(str);
}
