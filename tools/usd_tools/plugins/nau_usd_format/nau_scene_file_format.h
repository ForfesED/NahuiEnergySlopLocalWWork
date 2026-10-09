// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#ifndef PXR_USD_NAUSCENE_FILE_FORMAT_H
#define PXR_USD_NAUSCENE_FILE_FORMAT_H
 
#include "pxr/pxr.h"
#include "pxr/usd/usd/api.h"
#include "pxr/usd/sdf/textFileFormat.h"
#include "pxr/base/tf/declarePtrs.h"
#include "pxr/base/tf/staticTokens.h"
#include "nau_api.h"

PXR_NAMESPACE_OPEN_SCOPE

#define USD_NAUSCENE_FILE_FORMAT_TOKENS \
    ((Id, "nausd_scene"))((Version, "1.0"))

TF_DECLARE_PUBLIC_TOKENS(UsdNauSceneFileFormatTokens, USD_FORMAT_API, USD_NAUSCENE_FILE_FORMAT_TOKENS);

TF_DECLARE_WEAK_AND_REF_PTRS(UsdNauSceneFileFormat);

class UsdNauSceneFileFormat : public SdfTextFileFormat
{
private:
    SDF_FILE_FORMAT_FACTORY_ACCESS;

    UsdNauSceneFileFormat();
    virtual ~UsdNauSceneFileFormat();

    friend class UsdUsdFileFormat;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // USDA_FILE_FORMAT_H
