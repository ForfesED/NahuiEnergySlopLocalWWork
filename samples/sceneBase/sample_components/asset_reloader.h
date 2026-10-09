// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <EASTL/vector.h>
#include "nau/scene/components/component.h"
#include "nau/scene/components/component_life_cycle.h"

namespace nau::sample
{
    class AssetReloader final : public scene::Component,
                                public scene::IComponentUpdate,
                                public scene::IComponentEvents
    {
    public:
        NAU_OBJECT(AssetReloader, scene::Component, scene::IComponentUpdate, scene::IComponentEvents)
        NAU_DECLARE_DYNAMIC_OBJECT
        NAU_CLASS_FIELDS(
            CLASS_NAMED_FIELD(m_assetPaths, "assetPaths"))

        void setAssetPath(eastl::string_view assetPath);

    private:
        void updateComponent(float dt) override;
        void onComponentActivated() override;


        eastl::vector<eastl::string> m_assetPaths;
        size_t m_currentSelection = 0;
    };
}  // namespace nau::sample
