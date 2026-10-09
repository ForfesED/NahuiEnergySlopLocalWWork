// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/app/platform_window.h"
#include "nau/render/render_window.h"
#include "nau/scene/components/component.h"
#include "nau/scene/components/component_life_cycle.h"

namespace nau::sample
{
    class WindowMaker final : public scene::Component,
                              public scene::IComponentUpdate
    {
    public:
        NAU_OBJECT(WindowMaker, scene::Component, scene::IComponentUpdate)
        NAU_DECLARE_DYNAMIC_OBJECT

    private:
        void updateComponent(float dt) override;

        async::Task<nau::render::IRenderWindow::WeakRef> m_task;
        async::Task<eastl::vector<scene::IWorld::WeakRef>> m_sceneLoaderTask;
        eastl::vector<nau::render::IRenderWindow::WeakRef> m_rendWindows;

        eastl::list<nau::Ptr<IPlatformWindow>> m_windows;

        eastl::vector<scene::IWorld::WeakRef> m_worlds;
    };
}  // namespace nau::sample
