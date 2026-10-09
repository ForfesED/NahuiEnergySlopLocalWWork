// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// input_system_impl.h

#pragma once
#include <EASTL/map.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/vector.h>
#include <gainput/gainput.h>

#include "nau/input_system.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/service/service.h"

namespace nau
{
    class InputControllerImpl : public IInputController
    {
    public:
        InputControllerImpl(const eastl::string& name, IInputDevice* device) :
            m_name(name),
            m_device(device)
        {
        }

        eastl::string getName() const override;

        IInputDevice* getDevice() override;

        void update(float dt) override;

    private:
        eastl::string m_name;
        IInputDevice* m_device = nullptr;
    };
}  // namespace nau
