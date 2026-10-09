// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_property_proxy.h"

#include <pxr/usd/sdf/schema.h>
#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/relationship.h>

namespace UsdProxy
{
    UsdProxyProperty::UsdProxyProperty(const PXR_NS::TfToken& propName, ProxyPrimContextPtr primContext) :
        m_propName(propName),
        m_primContext(primContext)
    {
    }

    UsdProxyProperty::UsdProxyProperty(const PXR_NS::TfToken& propName, ProxyPrimContextPtr primContext, PXR_NS::SdfPropertySpecHandle specHandle) :
        m_propName(propName),
        m_primContext(primContext),
        m_propertySpec(specHandle)
    {
    }

    void UsdProxyProperty::setPropertySpec(PXR_NS::SdfPropertySpecHandle propertySpec)
    {
        m_propertySpec = propertySpec;
    }

    bool UsdProxyProperty::setValue(const PXR_NS::VtValue& value, PXR_NS::UsdTimeCode time /*= PXR_NS::UsdTimeCode::Default()*/)
    {
        if (!m_primContext)
            return false;

        auto m_type = getType();
        if (m_type == PXR_NS::SdfSpecType::SdfSpecTypeAttribute)
        {
            if (auto attribute = m_primContext->m_prim.GetAttribute(m_propName))
            {
                return attribute.Set(value, time);
            }
            else
            {
                PXR_NS::SdfChangeBlock block;
                auto prim = m_primContext->m_prim;
                auto editTarget = prim.GetStage()->GetEditTarget();
                auto targetPath = editTarget.MapToSpecPath(prim.GetPath());
                auto primSpec = targetPath.IsEmpty() ? PXR_NS::SdfPrimSpecHandle() : PXR_NS::SdfCreatePrimInLayer(editTarget.GetLayer(), targetPath);
                auto attrSpec = primSpec->GetAttributeAtPath(targetPath.AppendProperty(m_propName));
                if (!attrSpec)
                {
                    attrSpec = PXR_NS::SdfAttributeSpec::New(primSpec, m_propName.GetString(), getTypeName());
                    if (!attrSpec)
                        return false;
                    if (time.IsDefault())
                    {
                        attrSpec->SetDefaultValue(value);
                    }
                    else
                    {
                        editTarget.GetLayer()->SetTimeSample(attrSpec->GetPath(), time.GetValue(), value);
                    }
                    return true;
                }
            }
        }
        else if (m_type == PXR_NS::SdfSpecType::SdfSpecTypeRelationship && value.IsHolding<PXR_NS::SdfPathVector>())
        {
            if (auto relationship = m_primContext->m_prim.GetRelationship(m_propName))
            {
                return value.IsHolding<PXR_NS::SdfPathVector>() ? relationship.SetTargets(value.UncheckedGet<PXR_NS::SdfPathVector>()) : false;
            }
            else
            {
                PXR_NS::SdfChangeBlock block;
                auto& prim = m_primContext->m_prim;
                auto editTarget = prim.GetStage()->GetEditTarget();
                auto targetPath = editTarget.MapToSpecPath(prim.GetPath());
                auto primSpec = targetPath.IsEmpty() ? PXR_NS::SdfPrimSpecHandle() : SdfCreatePrimInLayer(editTarget.GetLayer(), targetPath);
                auto relationshipSpec = primSpec->GetRelationshipAtPath(targetPath.AppendProperty(m_propName));
                if (!relationshipSpec)
                {
                    relationshipSpec = PXR_NS::SdfRelationshipSpec::New(primSpec, m_propName.GetString(), false);
                    auto pathEditor = relationshipSpec->GetTargetPathList();
                    pathEditor.ClearEditsAndMakeExplicit();
                    pathEditor.GetExplicitItems() = value.UncheckedGet<PXR_NS::SdfPathVector>();
                    return true;
                }
            }
        }

        return false;
    }

    bool UsdProxyProperty::getValue(PXR_NS::VtValue* value, PXR_NS::UsdTimeCode time /*= PXR_NS::UsdTimeCode::Default()*/) const
    {
        if (!m_primContext)
            return false;

        if (auto prop = m_primContext->m_prim.GetProperty(m_propName))
        {
            if (prop.Is<PXR_NS::UsdAttribute>())
            {
                auto attr = prop.As<PXR_NS::UsdAttribute>();
                return attr.Get(value, time);
            }
            else if (prop.Is<PXR_NS::UsdRelationship>())
            {
                PXR_NS::SdfPathVector result;
                auto rel = prop.As<PXR_NS::UsdRelationship>();
                if (rel.GetTargets(&result))
                {
                    *value = PXR_NS::VtValue(result);
                    return true;
                }
            }

            return false;
        }

        return getDefault(value);
    }

    bool UsdProxyProperty::getDefault(PXR_NS::VtValue* value) const
    {
        if (!m_primContext)
            return false;

        if (auto prop = m_primContext->m_prim.GetProperty(m_propName))
        {
            if (!prop.Is<PXR_NS::UsdAttribute>())
            {
                *value = PXR_NS::VtValue(PXR_NS::SdfPathVector());
                return true;
            }

            if (!prop.GetMetadata(PXR_NS::SdfFieldKeys->Default, value))
            {
                PXR_NS::TfToken typeName;
                if (prop.GetMetadata(PXR_NS::SdfFieldKeys->TypeName, &typeName))
                {
                    if (auto valueType = PXR_NS::SdfSchema::GetInstance().FindType(typeName))
                    {
                        *value = valueType.GetDefaultValue();
                        return true;
                    }
                }
                return false;
            }
        }
        else if (m_propertySpec)
        {
            *value = m_propertySpec->GetDefaultValue();
            return true;
        }
        else
        {
            auto extraPropIt = m_primContext->m_extraProperties.find(m_propName);
            if (extraPropIt == m_primContext->m_extraProperties.end())
                return false;
            auto& meta = extraPropIt->second->m_metadata;

            auto valIt = meta.find(PXR_NS::SdfFieldKeys->Default);
            if (valIt != meta.end())
            {
                *value = valIt->second;
                return true;
            }

            valIt = meta.find(PXR_NS::SdfFieldKeys->TypeName);
            if (valIt != meta.end() && valIt->second.CanCast<PXR_NS::TfToken>())
            {
                PXR_NS::TfToken typeName = valIt->second.Get<PXR_NS::TfToken>();
                if (auto valueType = PXR_NS::SdfSchema::GetInstance().FindType(typeName))
                {
                    *value = valueType.GetDefaultValue();
                    return true;
                }
            }
        }
        return false;
    }

    bool UsdProxyProperty::getMetadata(const PXR_NS::TfToken& key, PXR_NS::VtValue& value) const
    {
        if (!m_primContext)
            return false;

        if (auto prop = m_primContext->m_prim.GetProperty(m_propName))
        {
            if (prop.GetMetadata(key, &value))
                return true;
        }

        if (m_propertySpec && m_propertySpec->HasField(key))
        {
            value = m_propertySpec->GetField(key);
            return true;
        }

        auto extraPropIt = m_primContext->m_extraProperties.find(m_propName);
        if (extraPropIt == m_primContext->m_extraProperties.end())
            return false;
        auto& meta = extraPropIt->second->m_metadata;
        auto valIt = meta.find(key);
        if (valIt != meta.end())
        {
            value = valIt->second;
            return true;
        }
        return false;
    }

    bool UsdProxyProperty::isAuthored() const
    {
        if (auto prop = m_primContext->m_prim.GetProperty(m_propName))
            return prop.IsAuthored();
        return false;
    }

    PXR_NS::UsdPrim UsdProxyProperty::getPrim() const
    {
        return m_primContext->m_prim;
    }

    PXR_NS::TfToken UsdProxyProperty::getNamespace() const
    {
        std::string const& fullName = getName().GetString();
        size_t delim = fullName.rfind(':');

        if (delim == fullName.size() - 1)
            return PXR_NS::TfToken();

        return ((delim == std::string::npos) ? PXR_NS::TfToken() : PXR_NS::TfToken(fullName.substr(0, delim)));
    }

    PXR_NS::SdfSpecType UsdProxyProperty::getType() const
    {
        if (auto prop = m_primContext->m_prim.GetProperty(m_propName))
        {
            return prop.Is<PXR_NS::UsdAttribute>()      ? PXR_NS::SdfSpecType::SdfSpecTypeAttribute
                   : prop.Is<PXR_NS::UsdRelationship>() ? PXR_NS::SdfSpecType::SdfSpecTypeRelationship
                                                        : PXR_NS::SdfSpecType::SdfSpecTypeUnknown;
        }

        if (m_propertySpec)
            return m_propertySpec->GetSpecType();

        auto extraPropIt = m_primContext->m_extraProperties.find(m_propName);
        if (extraPropIt == m_primContext->m_extraProperties.end())
            return PXR_NS::SdfSpecType::SdfSpecTypeUnknown;
        return extraPropIt->second->m_type;
    }

    PXR_NS::SdfValueTypeName UsdProxyProperty::getTypeName() const
    {
        if (auto prop = m_primContext->m_prim.GetProperty(m_propName))
        {
            PXR_NS::TfToken type_name;
            if (prop.GetMetadata(PXR_NS::SdfFieldKeys->TypeName, &type_name))
                return PXR_NS::SdfSchema::GetInstance().FindType(type_name);

            PXR_NS::VtValue value;
            if (prop.GetMetadata(PXR_NS::SdfFieldKeys->Default, &value))
                return PXR_NS::SdfSchema::GetInstance().FindType(value);

            return PXR_NS::SdfValueTypeName();
        }

        if (m_propertySpec)
            return m_propertySpec->GetTypeName();

        auto extraPropIt = m_primContext->m_extraProperties.find(m_propName);
        if (extraPropIt == m_primContext->m_extraProperties.end())
            return PXR_NS::SdfValueTypeName();

        auto& meta = extraPropIt->second->m_metadata;
        auto valIt = meta.find(PXR_NS::SdfFieldKeys->TypeName);
        if (valIt != meta.end() && valIt->second.CanCast<PXR_NS::TfToken>())
        {
            PXR_NS::TfToken typeName = valIt->second.Get<PXR_NS::TfToken>();
            return PXR_NS::SdfSchema::GetInstance().FindType(typeName);
        }

        valIt = meta.find(PXR_NS::SdfFieldKeys->Default);
        if (valIt != meta.end())
        {
            return PXR_NS::SdfSchema::GetInstance().FindType(valIt->second);
        }

        return PXR_NS::SdfValueTypeName();
    }

    PXR_NS::TfToken UsdProxyProperty::getName() const
    {
        return m_propName;
    }

    PXR_NS::SdfPropertySpecHandle UsdProxyProperty::getPropertySpec() const
    {
        return m_propertySpec;
    }

    bool UsdProxyProperty::isValid() const
    {
        return static_cast<bool>(m_primContext) && (m_primContext->m_prim.HasProperty(m_propName) || m_propertySpec || m_primContext->m_extraProperties.find(m_propName) != m_primContext->m_extraProperties.end());
    }

    UsdProxyProperty::operator bool() const
    {
        return isValid();
    }
}  // namespace UsdProxy
