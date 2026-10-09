// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/import_settings_provider.h"

namespace nau
{
    /**
     */
    struct TextureImportSettingsProvider : public IImportSettingsProvider
    {
        NAU_INTERFACE(nau::TextureImportSettingsProvider, IImportSettingsProvider)

        RuntimeReadonlyDictionary::Ptr getAssetImportSettings(const io::FsPath& assetPath, const IAssetContainerLoader& loader);
    };
}  // namespace nau
