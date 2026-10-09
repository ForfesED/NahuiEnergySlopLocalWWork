// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "pxr/pxr.h"
#include "nau_usd_file_format.h"

#include "pxr/usd/usd/usdFileFormat.h"

#include "pxr/usd/sdf/layer.h"
#include "pxr/base/tf/registryManager.h"

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PUBLIC_TOKENS(UsdNauUsdFileFormatTokens, USD_NAUUSD_FILE_FORMAT_TOKENS);

NAUTF_REGISTRY_FUNCTION_NAMED(UsdNauUsdFileFormat)
{
    SDF_DEFINE_FILE_FORMAT(UsdNauUsdFileFormat, SdfTextFileFormat);
}

UsdNauUsdFileFormat::UsdNauUsdFileFormat()
    : SdfTextFileFormat(UsdNauUsdFileFormatTokens->Id,
                        UsdNauUsdFileFormatTokens->Version,
                        UsdUsdFileFormatTokens->Target)
{
    NAU_TOUCH_REGISTRY(UsdNauUsdFileFormat);
    // Do Nothing.
}

UsdNauUsdFileFormat::~UsdNauUsdFileFormat()
{
    // Do Nothing.
}

PXR_NAMESPACE_CLOSE_SCOPE

