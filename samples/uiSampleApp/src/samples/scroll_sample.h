// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <filesystem>
#include "nau/animation/components/animation_component.h"
#include "nau/animation/playback/animation_transforms.h"
#include "nau/animation/playback/animation_graphic_props.h"
#include "nau/io/virtual_file_system.h"
#include "nau/math/math.h"
#include "nau/math/transform.h"
#include "nau/module/module_manager.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/scene/scene.h"
#include "nau/scene/scene_factory.h"
#include "nau/scene/scene_manager.h"
#include "nau/service/service_provider.h"
#include "nau/ui.h"
#include "nau/ui/label.h"
#include "nau/ui/button.h"
#include "nau/ui/scroll.h"
#include "nau/ui/button_data.h"
#include "nau/ui/elements/canvas.h"
#include "nau/ui/elements/node.h"
#include "nau/ui/elements/sprite.h"
#include "nau/ui/elements/draw_node.h"
#include "nau/input.h"

#include "renderer/CCTextureCache.h"
#include "platform/CCFileUtils.h"

namespace nau::sample
{
    async::Task<> scrollScene(nau::ui::Canvas* sampleUiScene);
}