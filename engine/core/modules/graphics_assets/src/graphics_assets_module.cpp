// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "graphics_assets/static_mesh_asset.h"
#include "graphics_assets/shader_asset.h"
#include "graphics_assets/texture_asset.h"
#include "graphics_assets/asset_view_factory.h"
#include "nau/module/module.h"

namespace nau
{
    struct GraphicsAssetsModule : public IModule
    {
        nau::string getModuleName() override
        {
            return "GraphicsAssetsModule";
        }
        void initialize() override
        {
            NAU_MODULE_EXPORT_CLASS(ShaderAssetView);
            NAU_MODULE_EXPORT_CLASS(StaticMeshAssetView);
            NAU_MODULE_EXPORT_CLASS(TextureAssetView);
            NAU_MODULE_EXPORT_SERVICE(GraphicsAssetViewFactory);
        }
        void deinitialize() override
        {
        }
        void postInit() override
        {
        }
    };
}  // namespace nau

IMPLEMENT_MODULE(nau::GraphicsAssetsModule);
