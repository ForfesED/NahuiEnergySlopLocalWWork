// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "pxr/usd/usd/prim.h"
#include "usd_proxy/usd_property_proxy.h"
#include "usd_proxy/usd_proxy_api.h"
#include "usd_proxy/usd_proxy_decorators_regestry.h"

namespace UsdProxy
{
    class USD_PROXY_API UsdProxyPrim
    {
    public:
        UsdProxyPrim(const PXR_NS::UsdPrim& prim);
        UsdProxyPrim() = default;
        UsdProxyPrim(const UsdProxyPrim&) = default;
        UsdProxyPrim(UsdProxyPrim&&) = default;
        UsdProxyPrim& operator=(const UsdProxyPrim&) = default;
        UsdProxyPrim& operator=(UsdProxyPrim&&) = default;

        UsdProxyPropertyPtr getProperty(const PXR_NS::TfToken& property_name) const;
        UsdProxyPropertyMap getProperties() const;
        PXR_NS::TfTokenVector getPrimSchemas() const;
        const PXR_NS::TfToken& getName() const;
        const PXR_NS::TfToken& getType() const;
        const PXR_NS::UsdPrimTypeInfo& getTypeInfo() const;
        const PXR_NS::UsdPrim& getPrim() const;
        bool isValid() const;
        operator bool() const;

    private:
        std::vector<const PXR_NS::UsdPrimDefinition*> getSchemasDefinitions() const;

        ProxyPrimContextPtr m_context;
    };

}  // namespace UsdProxy
