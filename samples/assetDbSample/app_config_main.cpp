// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <nau/scene/components/skinned_mesh_component.h>
#include <nau/scene/components/static_mesh_component.h>

#include <filesystem>

#include "nau/app/run_application.h"
#include "nau/assets/asset_db.h"
#include "nau/assets/scene_asset.h"
#include "nau/input.h"
#include "nau/io/virtual_file_system.h"
#include "nau/module/module_manager.h"
#include "nau/samples/sample_app_delegate.h"
#include "nau/scene/scene.h"
#include "nau/scene/scene_factory.h"
#include "nau/scene/scene_manager.h"
#include "nau/scene/scene_object.h"
#include "nau/service/service_provider.h"

namespace nau
{
    namespace sample
    {
        class AssetDbSampleDelegate final : public SampleAppDelegate
        {
        public:
            AssetDbSampleDelegate() :
                SampleAppDelegate("assetDbSample")
            {
            }

            Result<> initializeServices() override
            {
                getServiceProvider().get<nau::IAssetDB>().addAssetDB("assets_db/database.db");
                return ResultSuccess;
            }

            async::Task<> startupApplication() override
            {
                using namespace nau::async;
                using namespace nau::scene;

                IScene::Ptr mainScene = co_await nau::scene::openScene("uid:24ef5f04-a9a9-11ef-9120-502f9ba726f4");

                mainScene->setName("Main");

                auto& sceneManager = getServiceProvider().get<scene::ISceneManager>();
                auto& sceneFactory = getServiceProvider().get<scene::ISceneFactory>();

                co_await sceneManager.activateScene(std::move(mainScene));
            }
        };

    }  // namespace sample
}  // namespace nau

int main(int argc, char** argv)
{
    return nau::runApplication(eastl::make_unique<nau::sample::AssetDbSampleDelegate>());
}
