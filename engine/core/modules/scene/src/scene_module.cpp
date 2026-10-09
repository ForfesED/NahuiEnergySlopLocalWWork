// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "camera/camera_manager_impl.h"
#include "nau/module/module.h"
#include "nau/scene/components/billboard_component.h"
#include "nau/scene/components/camera_component.h"
#include "nau/scene/components/omnilight_component.h"
#include "nau/scene/components/skinned_mesh_component.h"
#include "nau/scene/components/static_mesh_component.h"
#include "nau/scene/components/billboard_component.h"
#include "nau/scene/components/directional_light_component.h"
#include "nau/scene/components/environment_component.h"
#include "nau/scene/components/omnilight_component.h"
#include "nau/scene/components/spotlight_component.h"
#include "scene_management/scene_factory_impl.h"
#include "scene_management/scene_manager_impl.h"

namespace nau::scene
{
    /**
     */
    class SceneModule : public DefaultModuleImpl
    {
        string getModuleName() override
        {
            return "nau.scene";
        }

        void initialize() override
        {
            NAU_MODULE_EXPORT_SERVICE(SceneFactoryImpl);
            NAU_MODULE_EXPORT_SERVICE(SceneManagerImpl);
            NAU_MODULE_EXPORT_SERVICE(CameraManagerImpl);

            NAU_MODULE_EXPORT_CLASS(SceneComponent);
            NAU_MODULE_EXPORT_CLASS(CameraComponent);
            NAU_MODULE_EXPORT_CLASS(StaticMeshComponent);
            NAU_MODULE_EXPORT_CLASS(SkinnedMeshComponent);
            NAU_MODULE_EXPORT_CLASS(BillboardComponent);
            NAU_MODULE_EXPORT_CLASS(DirectionalLightComponent);
            NAU_MODULE_EXPORT_CLASS(EnvironmentComponent);
            NAU_MODULE_EXPORT_CLASS(OmnilightComponent);
            NAU_MODULE_EXPORT_CLASS(SpotlightComponent);
        }
    };
}  // namespace nau::scene

IMPLEMENT_MODULE(nau::scene::SceneModule)
