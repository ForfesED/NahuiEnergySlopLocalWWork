// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/meta/attribute.h"

namespace nau::scene
{
    NAU_DEFINE_ATTRIBUTE(ComponentDisplayNameAttrib, "nau.scene.component_display_name", meta::AttributeOptionsNone)

    NAU_DEFINE_ATTRIBUTE(ComponentDescriptionAttrib, "nau.scene.component_description", meta::AttributeOptionsNone)

    NAU_DEFINE_ATTRIBUTE(HiddenAttributeAttr, "nau.scene.hidden_component", meta::AttributeOptionsNone)
}  // namespace nau::scene
