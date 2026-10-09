// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "usd_translator/usd_translator_api.h"

#include <pxr/usd/usd/prim.h>
#include <map>

#include "nau/ui.h"
#include "nau/ui/elements/canvas.h"
#include "nau/datablock/dag_dataBlock.h"

namespace  UsdTranslator
{
    class USD_TRANSLATOR_API IUIPrimAdapter
    {
    public:
        using Ptr = std::shared_ptr<IUIPrimAdapter>;

        IUIPrimAdapter(PXR_NS::UsdPrim prim);
        virtual ~IUIPrimAdapter();

        PXR_NS::UsdPrim getPrim() const;
        PXR_NS::SdfPath getPrimPath() const;

        const std::map<PXR_NS::TfToken, Ptr>& getChildren() const;
        Ptr getChild(PXR_NS::TfToken name) const;
        void addChild(PXR_NS::TfToken name, Ptr adapter);
        void destroyChild(PXR_NS::TfToken name);

        void serializeToBlk(nau::DataBlock& blk);
        void toBlk(nau::DataBlock &blk);
        virtual std::string getType() const { return "none"; };
        virtual void serializeChildren(nau::DataBlock& blk);
        virtual void serializeNodeContent(nau::DataBlock& blk);

        virtual nau::Uid getUid() const = 0;

        // initialize SceneObject if need return new child
        virtual nau::ui::Node* initializeNode() = 0;
        virtual nau::ui::Node* getNode() const = 0;

        virtual void update() = 0;
        virtual bool isValid() const = 0;

        // TODO: Implement
        //virtual void release() = 0;
        //virtual void setActive(bool val) = 0;
        //virtual void isActive() const = 0;

        // TODO: NAU-2121
        // Make a generic “addChildInternal” method for the “IUIPrimAdapter” interface
        virtual void addChildInternal(nau::ui::Node* node) = 0;

    protected:
        // TODO: NAU-2120
        // Make a generic “destroyNode” method for the “IUIPrimAdapter” interface
        virtual void destroyNode() = 0;

        const std::string getSourcePath(const PXR_NS::SdfAssetPath& sdfPath);

        // TODO: Temporary. This code duplicates NauUsdAssetProcessor:sourcePathFromAssetFile.
        static std::string sourcePathFromAssetFile(const std::string& assetPath);

    private:
        void destroy();

        PXR_NS::SdfPath m_path;
        PXR_NS::UsdPrim m_prim;

        std::map<PXR_NS::TfToken, Ptr> m_children;
    };
}
