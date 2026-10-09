// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <memory>

#include "pxr/usd/usd/common.h"
#include "pxr/usd/usd/prim.h"
#include "pxr/usd/usd/property.h"
#include "usd_proxy/usd_proxy_api.h"

namespace UsdProxy
{
    struct ProxyPrimContext;
    struct ProxyPropertyContext;
    class IUsdProxyPrimDecorator;

    using ProxyPrimContextPtr = std::shared_ptr<ProxyPrimContext>;
    using ProxyPropertyContextPtr = std::shared_ptr<ProxyPropertyContext>;
    using IUsdProxyPrimDecoratorPtr = std::shared_ptr<IUsdProxyPrimDecorator>;

    struct USD_PROXY_API ProxyPrimContext
    {
        friend class UsdProxyPrim;
        friend class UsdProxyProperty;

        ProxyPrimContext(PXR_NS::UsdPrim prim);
        ProxyPrimContext(const ProxyPrimContext&) = default;
        ProxyPrimContext(ProxyPrimContext&&) = default;
        ProxyPrimContext& operator=(const ProxyPrimContext&) = default;
        ProxyPrimContext& operator=(ProxyPrimContext&&) = default;

        bool tryInsertProperty(ProxyPropertyContextPtr prop);
        bool tryInsertMetadata(const PXR_NS::TfToken& name, const PXR_NS::VtValue& value);
        bool tryInsertMetadata(const std::string& name, const PXR_NS::VtValue& value);

        PXR_NS::UsdPrim getPrim() const;

    private:
        using propertyMap = std::map<PXR_NS::TfToken, ProxyPropertyContextPtr, PXR_NS::TfDictionaryLessThan>;
        ProxyPropertyContextPtr getProperty(const PXR_NS::TfToken& name);

        PXR_NS::UsdPrim m_prim;
        propertyMap m_extraProperties;
        PXR_NS::UsdMetadataValueMap m_metadata;
    };

    struct USD_PROXY_API ProxyPropertyContext
    {
        friend struct ProxyPrimContext;
        friend class UsdProxyProperty;
        friend class UsdProxyPrim;

        ProxyPropertyContext(PXR_NS::SdfSpecType type, const PXR_NS::TfToken& name, const PXR_NS::VtValue& defaultValue, const PXR_NS::UsdMetadataValueMap& metadata);
        ProxyPropertyContext() = default;
        ProxyPropertyContext(const ProxyPropertyContext&) = default;
        ProxyPropertyContext(ProxyPropertyContext&&) = default;
        ProxyPropertyContext& operator=(const ProxyPropertyContext&) = default;
        ProxyPropertyContext& operator=(ProxyPropertyContext&&) = default;

        ProxyPropertyContext& setName(const PXR_NS::TfToken& name);
        ProxyPropertyContext& setType(PXR_NS::SdfSpecType type);
        ProxyPropertyContext& setDefaultValue(const PXR_NS::VtValue& defaultValue);
        ProxyPropertyContext& setMetadata(const PXR_NS::TfToken& key, const PXR_NS::VtValue& value);

    private:
        PXR_NS::UsdPrim m_prim;
        PXR_NS::SdfSpecType m_type = PXR_NS::SdfSpecType::SdfSpecTypeAttribute;
        PXR_NS::TfToken m_name;
        PXR_NS::UsdMetadataValueMap m_metadata;
    };

    class USD_PROXY_API IUsdProxyPrimDecorator
    {
    public:
        virtual ~IUsdProxyPrimDecorator()
        {
        }
        virtual void decorate(ProxyPrimContextPtr context) = 0;
    };

    class USD_PROXY_API ProxyDecoratorsRegestry
    {
    public:
        static ProxyDecoratorsRegestry& instance();

        [[nodiscard]] bool addPrimDecorator(IUsdProxyPrimDecoratorPtr decorator);
        void decorate(ProxyPrimContextPtr context);

    private:
        ProxyDecoratorsRegestry() = default;
        ProxyDecoratorsRegestry(const ProxyDecoratorsRegestry&) = default;
        ProxyDecoratorsRegestry(ProxyDecoratorsRegestry&&) = default;
        ProxyDecoratorsRegestry& operator=(const ProxyDecoratorsRegestry&) = default;
        ProxyDecoratorsRegestry& operator=(ProxyDecoratorsRegestry&&) = default;

        std::mutex m_sync;
        std::vector<IUsdProxyPrimDecoratorPtr> m_decorators;
    };
}  // namespace UsdProxy

#define REGISTRY_PROXY_DECORATOR(DecoratorType) bool ProxyRegistered##DecoratorType = UsdProxy::ProxyDecoratorsRegestry::instance().addPrimDecorator(std::make_shared<DecoratorType>())
USD_PROXY_API PXR_NS::TfToken operator""_tftoken(const char* str, size_t val);
