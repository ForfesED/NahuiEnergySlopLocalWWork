// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/io/fs_path.h"
#include "nau/rtti/type_info.h"
#include "nau/serialization/runtime_value.h"

namespace nau
{
    /**
     */
    struct IImportSettingsProvider
    {
        NAU_TYPEID(IImportSettingsProvider)

        virtual ~IImportSettingsProvider() = default;

        virtual RuntimeReadonlyDictionary::Ptr getAssetImportSettings(const io::FsPath& assetPath, const struct IAssetContainerLoader&) = 0;
    };
}  // namespace nau
