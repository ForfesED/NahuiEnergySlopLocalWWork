// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_prim_proxy.h"

namespace UsdProxy
{
    UsdProxyPrim::UsdProxyPrim(const PXR_NS::UsdPrim& prim) :
        m_context(std::make_shared<ProxyPrimContext>(prim))
    {
        ProxyDecoratorsRegestry::instance().decorate(m_context);
    }

    PXR_NS::TfTokenVector UsdProxyPrim::getPrimSchemas() const
    {
        auto appliedSchemas = m_context->m_prim.GetAppliedSchemas();
        PXR_NS::TfTokenVector schemaNames(appliedSchemas.size() + 1);
        schemaNames[0] = m_context->m_prim.GetTypeName();
        std::move(appliedSchemas.begin(), appliedSchemas.end(), schemaNames.begin() + 1);
        return std::move(schemaNames);
    }

    std::vector<const PXR_NS::UsdPrimDefinition*> UsdProxyPrim::getSchemasDefinitions() const
    {
        const auto applied_schemas = m_context->m_prim.GetPrimDefinition().GetAppliedAPISchemas();
        std::vector<const PXR_NS::UsdPrimDefinition*> result(applied_schemas.size() + 1);
        result[0] = &m_context->m_prim.GetPrimDefinition();
        std::transform(applied_schemas.begin(), applied_schemas.end(), result.begin() + 1, [](const PXR_NS::TfToken& schema_name)
        {
            return PXR_NS::UsdSchemaRegistry::GetInstance().FindAppliedAPIPrimDefinition(schema_name);
        });
        return std::move(result);
    }

    UsdProxyPropertyPtr UsdProxyPrim::getProperty(const PXR_NS::TfToken& property_name) const
    {
        auto dest = std::make_shared<UsdProxyProperty>(property_name, m_context);
        if (dest->isValid())
            return dest;

        return nullptr;
    }

    UsdProxyPropertyMap UsdProxyPrim::getProperties() const
    {
        UsdProxyPropertyMap propertyMap;

        for (auto prop : m_context->m_prim.GetProperties())
            propertyMap.emplace(prop.GetName(), std::make_shared<UsdProxyProperty>(prop.GetName(), m_context));

        for (auto exProp : m_context->m_extraProperties)
            if (propertyMap.find(exProp.first) == propertyMap.end())
                propertyMap.emplace(exProp.first, std::make_shared<UsdProxyProperty>(exProp.first, m_context));

        for (const auto schemaDef : getSchemasDefinitions())
        {
            if (!schemaDef)
                continue;
            for (const auto propertyName : schemaDef->GetPropertyNames())
            {
                if (auto propSpec = schemaDef->GetSchemaPropertySpec(propertyName))
                {
                    if (auto it = propertyMap.find(propertyName); it != propertyMap.end())
                    {
                        it->second->setPropertySpec(propSpec);
                    }
                    else
                    {
                        const auto source = std::make_shared<UsdProxyProperty>(propertyName, m_context, propSpec);
                        propertyMap.emplace(propertyName, source);
                    }
                }
            }
        }

        return std::move(propertyMap);
    }

    const PXR_NS::TfToken& UsdProxyPrim::getName() const
    {
        return getPrim().GetName();
    }

    const PXR_NS::TfToken& UsdProxyPrim::getType() const
    {
        return getPrim().GetPrimTypeInfo().GetTypeName();
    }

    const PXR_NS::UsdPrimTypeInfo& UsdProxyPrim::getTypeInfo() const
    {
        return getPrim().GetPrimTypeInfo();
    }

    const PXR_NS::UsdPrim& UsdProxyPrim::getPrim() const
    {
        return m_context->m_prim;
    }

    bool UsdProxyPrim::isValid() const
    {
        return static_cast<bool>(m_context);
    }

    UsdProxyPrim::operator bool() const
    {
        return isValid();
    }
}  // namespace UsdProxy
