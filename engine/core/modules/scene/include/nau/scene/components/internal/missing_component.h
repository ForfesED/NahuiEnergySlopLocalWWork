// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/scene_asset.h"
#include "nau/scene/components/component.h"

namespace nau::scene
{
    /**
     */
    struct NAU_ABSTRACT_TYPE IMissingComponent
    {
        NAU_TYPEID(nau::scene::IMissingComponent)

        virtual ~IMissingComponent() = default;

        virtual void setComponentData(const ComponentAsset& componentData) = 0;

        virtual void fillComponentData(ComponentAsset& componentData) = 0;
    };

    /**
     */
    NAU_CORESCENE_EXPORT ObjectUniquePtr<Component> createDefaultMissingComponent();
}  // namespace nau::scene
