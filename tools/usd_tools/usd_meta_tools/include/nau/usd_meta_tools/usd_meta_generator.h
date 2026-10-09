// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/usd_meta_tools/usd_meta_api.h"

#include <filesystem>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <nau/utils/uid.h>

namespace nau
{
    using MetaArgs = std::map<PXR_NS::TfToken, PXR_NS::VtValue>;

    class IMetaGeneratorTemplate
    {
    public:
        using Ptr = std::shared_ptr<IMetaGeneratorTemplate>;

        virtual ~IMetaGeneratorTemplate() {};
        virtual const nau::MetaArgs& getDefaultArgs() const = 0;
        virtual bool generate(PXR_NS::UsdStagePtr stage, const MetaArgs& args) = 0;
    };

    class IMetaGenerator
    {
    public:
        using Ptr = std::shared_ptr<IMetaGenerator>;

        virtual ~IMetaGenerator() {};
        virtual const nau::MetaArgs& getDefaultArgs() const = 0;
        virtual bool generate(const std::filesystem::path& path, PXR_NS::UsdStagePtr stage, const MetaArgs& args) = 0;
    };

    class IPrimMetaGenerator
    {
    public:
        using Ptr = std::shared_ptr<IPrimMetaGenerator>;

        virtual ~IPrimMetaGenerator() {};
        virtual const nau::MetaArgs& getDefaultArgs() const = 0;
        virtual PXR_NS::UsdPrim generate(PXR_NS::UsdPrim source, PXR_NS::UsdStagePtr stage, const PXR_NS::SdfPath& dest, const MetaArgs& args) = 0;
    };
    
    class USD_META_TOOLS_API UsdMetaGenerator
    {
    public:
        static UsdMetaGenerator& instance();

        bool addAssetTemplate(const std::string& templateName, IMetaGeneratorTemplate::Ptr generator);
        bool addFileGenerator(const std::set<std::string>& extentions, IMetaGenerator::Ptr generator);
        bool addPrimGenerator(const PXR_NS::TfToken& primType, IPrimMetaGenerator::Ptr generator);

        [[nodiscard]] bool generateAssetTemplate(const std::filesystem::path& dest, const std::string& templateName, const MetaArgs& args);
        [[nodiscard]] PXR_NS::UsdStageRefPtr generate(const std::filesystem::path& path, MetaArgs args = {});
        [[nodiscard]] bool write(const std::filesystem::path& path, PXR_NS::UsdStageRefPtr stage) const;
        std::map<PXR_NS::TfToken, nau::MetaArgs> getArgs(const std::filesystem::path& path) const;
        bool canGenerate(const std::filesystem::path& path) const;
    private:
        [[nodiscard]] PXR_NS::UsdPrim generate(const std::filesystem::path& sourceAssetPath, PXR_NS::UsdPrim source, PXR_NS::UsdStagePtr stage, const PXR_NS::SdfPath& dest, const MetaArgs& args);

        PXR_NS::UsdStageRefPtr processAsset(const std::string& ext, const std::filesystem::path& path, const MetaArgs& args);
        PXR_NS::UsdStageRefPtr processContainer(const std::filesystem::path& path, const MetaArgs& arg);

        UsdMetaGenerator() = default;
        UsdMetaGenerator(const UsdMetaGenerator&) = delete;
        UsdMetaGenerator(UsdMetaGenerator&&) = delete;
        UsdMetaGenerator& operator=(const UsdMetaGenerator&) = delete;
        UsdMetaGenerator& operator=(UsdMetaGenerator&&) = delete;

        std::map<std::string, IMetaGenerator::Ptr> m_fileRegistry;
        std::map<PXR_NS::TfToken, IPrimMetaGenerator::Ptr> m_primRegistry;
        std::map<std::string, IMetaGeneratorTemplate::Ptr> m_templateRegistry;

    };
}

#define DeclareMetaGenerator(ClassGen, ...) nau::UsdMetaGenerator::instance().addFileGenerator(__VA_ARGS__, std::make_shared<ClassGen>())
#define DeclareMetaTemplate(ClassGen, templateName) nau::UsdMetaGenerator::instance().addAssetTemplate(templateName, std::make_shared<ClassGen>())
#define DeclarePrimMetaGenerator(ClassGen, primType) nau::UsdMetaGenerator::instance().addPrimGenerator(PXR_NS::TfToken(primType), std::make_shared<ClassGen>())