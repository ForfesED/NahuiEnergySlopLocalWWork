// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include "nau/scene/components/component.h"

namespace nau::scene
{
    /**
     */
    struct NAU_ABSTRACT_TYPE IComponentFactory
    {
        NAU_TYPEID(nau::scene::IComponentFactory)

        virtual ObjectUniquePtr<Component> createComponent(const rtti::TypeInfo& type) = 0;
    };

}  // namespace nau
