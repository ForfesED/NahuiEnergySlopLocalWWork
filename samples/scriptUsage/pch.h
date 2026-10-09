// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <imgui.h>

#include <filesystem>
#include <iostream>

#include "nau/app/application.h"
#include "nau/app/application_services.h"
#include "nau/app/platform_window.h"
#include "nau/app/window_manager.h"
#include "nau/assets/asset_container.h"
#include "nau/assets/asset_container_builder.h"
#include "nau/assets/asset_manager.h"
#include "nau/assets/asset_ref.h"
#include "nau/assets/scene_asset.h"
#include "nau/diag/assertion.h"
#include "nau/diag/logging.h"
#include "nau/io/asset_pack_file_system.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/scene/components/camera_component.h"
#include "nau/scene/components/component_life_cycle.h"
#include "nau/scene/components/skinned_mesh_component.h"
#include "nau/scene/components/static_mesh_component.h"
#include "nau/scene/scene.h"
#include "nau/scene/scene_factory.h"
#include "nau/scene/scene_manager.h"
#include "nau/service/service_provider.h"
#include "nau/utils/enum/enum_reflection.h"
#include "nau/utils/performance_profiling.h"
