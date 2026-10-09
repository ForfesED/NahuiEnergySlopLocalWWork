// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/animation/controller/animation_controller_direct.h"
#include "nau/animation/animation_manager.h"
#include "nau/animation/animation_scene_processor.h"
#include "nau/animation/components/animation_component.h"
#include "nau/animation/components/skeleton_component.h"
#include "nau/animation/components/skeleton_socket_component.h"
#include "assets/asset_view_factory.h"
#include "nau/module/module.h"

namespace nau
{
    /**
     */
    class AnimationModule : public DefaultModuleImpl
    {
        string getModuleName() override
        {
            return "nau.animation";
        }

        void initialize() override
        {
            NAU_MODULE_EXPORT_CLASS(animation::AnimationComponent);
            NAU_MODULE_EXPORT_CLASS(animation::AnimationManager);
            NAU_MODULE_EXPORT_CLASS(animation::AnimationSceneProcessor);

            NAU_MODULE_EXPORT_CLASS(animation::DirectAnimationController);
            NAU_MODULE_EXPORT_SERVICE(animation::data::AnimationAssetViewFactory);

            NAU_MODULE_EXPORT_CLASS(SkeletonComponent);
            NAU_MODULE_EXPORT_CLASS(SkeletonSocketComponent);

            getServiceProvider().addService(rtti::createInstance<animation::AnimationSceneProcessor>());
        }
    };
}  // namespace nau

IMPLEMENT_MODULE(nau::AnimationModule)
