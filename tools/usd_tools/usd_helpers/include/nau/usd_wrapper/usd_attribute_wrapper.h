// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/serialization/runtime_value.h"
#include "pxr/usd/usd/attribute.h"

namespace nau
{
    NAU_USDHELPERS_EXPORT
    RuntimeValue::Ptr attributeAsRuntimeValue(PXR_NS::UsdAttribute& attribute);

    NAU_USDHELPERS_EXPORT
    Result<> runtimeApplyToAttributeValue(PXR_NS::UsdAttribute& attribute, const RuntimeValue::Ptr& runtimeValue);
    
    NAU_USDHELPERS_EXPORT
    Result<> createAttributeByValue(PXR_NS::UsdPrim prim, PXR_NS::TfToken attributeName, const RuntimeValue::Ptr& runtimeValue);
}  // namespace nau
