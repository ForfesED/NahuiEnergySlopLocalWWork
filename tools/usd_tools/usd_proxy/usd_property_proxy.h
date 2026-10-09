// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "usd_proxy/usd_proxy_api.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

namespace UsdProxy
{
    class USD_PROXY_API UsdProxyProperty
    {
    public:
        UsdProxyProperty(const PXR_NS::TfToken& propName, ProxyPrimContextPtr primContext);
        UsdProxyProperty(const PXR_NS::TfToken& propName, ProxyPrimContextPtr primContext, PXR_NS::SdfPropertySpecHandle specHandle);
        UsdProxyProperty() = default;
        UsdProxyProperty(const UsdProxyProperty&) = default;
        UsdProxyProperty(UsdProxyProperty&&) = default;
        UsdProxyProperty& operator=(const UsdProxyProperty&) = default;
        UsdProxyProperty& operator=(UsdProxyProperty&&) = default;

        void setPropertySpec(PXR_NS::SdfPropertySpecHandle propertySpec);
        bool setValue(const PXR_NS::VtValue& value, PXR_NS::UsdTimeCode time = PXR_NS::UsdTimeCode::Default());
        [[nodiscard]] bool getValue(PXR_NS::VtValue* value, PXR_NS::UsdTimeCode time = PXR_NS::UsdTimeCode::Default()) const;
        [[nodiscard]] bool getDefault(PXR_NS::VtValue* value) const;
        [[nodiscard]] bool getMetadata(const PXR_NS::TfToken& key, PXR_NS::VtValue& value) const;
        bool isAuthored() const;
        PXR_NS::UsdPrim getPrim() const;
        PXR_NS::TfToken getNamespace() const;
        PXR_NS::SdfSpecType getType() const;
        PXR_NS::SdfValueTypeName getTypeName() const;
        PXR_NS::TfToken getName() const;
        PXR_NS::SdfPropertySpecHandle getPropertySpec() const;

        bool isValid() const;
        operator bool() const;

    private:
        PXR_NS::TfToken m_propName;
        PXR_NS::SdfPropertySpecHandle m_propertySpec;
        ProxyPrimContextPtr m_primContext;
    };

    using UsdProxyPropertyPtr = std::shared_ptr<UsdProxyProperty>;
    using UsdProxyPropertyVector = std::vector<UsdProxyPropertyPtr>;
    using UsdProxyPropertyMap = std::map<PXR_NS::TfToken, UsdProxyPropertyPtr>;
}  // namespace UsdProxy


namespace std
{
    template class USD_PROXY_API map<PXR_NS::TfToken, UsdProxy::UsdProxyPropertyPtr>;
}
