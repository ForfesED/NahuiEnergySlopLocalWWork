// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/usd_meta_tools/usd_meta_api.h"
#include "nau/usd_meta_tools/usd_meta_info.h"

#include <string>
#include <functional>
#include <map>

#include <pxr/usd/usd/prim.h>

namespace nau
{
    class USD_META_TOOLS_API IMetaProcessor
    {
    public:
        virtual ~IMetaProcessor() = default;
        virtual bool process(PXR_NS::UsdPrim prim, UsdMetaInfo& dest) = 0;
    };

    class USD_META_TOOLS_API UsdMetaManager
    {
    public:
        static UsdMetaManager& instance();

        UsdMetaInfoArray getInfo(PXR_NS::UsdStageRefPtr stage) const;
        UsdMetaInfoArray getInfo(const std::string& filePath) const;

        UsdMetaInfo getPrimInfo(PXR_NS::UsdPrim& prim) const;

        bool addProcessor(const PXR_NS::TfToken& primType, IMetaProcessor* processor);
        const IMetaProcessor* getProcessor(const PXR_NS::TfToken& primType) const;
        const bool isValidMeta(const UsdMetaInfo& meta) const;
    private:
        UsdMetaManager() = default;
        UsdMetaManager(const UsdMetaManager&) = delete;
        UsdMetaManager(UsdMetaManager&&) = delete;
        UsdMetaManager& operator=(const UsdMetaManager&) = delete;
        UsdMetaManager& operator=(UsdMetaManager&&) = delete;

        std::map<PXR_NS::TfToken, IMetaProcessor*> m_regestry;
    };

    void USD_META_TOOLS_API loadPlugins(); // based on usd plugin system

}
#define DefineNauMetaProcessor(primType, MetaProcessor) nau::UsdMetaManager::instance().addProcessor(primType, new MetaProcessor());
#define DefineNauMetaPlugin extern "C" __declspec(dllexport) void nau_meta_plugin_entry()