// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "./texture_import_settings.h"

#include "nau/io/file_system.h"
#include "nau/serialization/json.h"
#include "nau/service/service_provider.h"
#include "nau/string/string_conv.h"

namespace nau
{
    RuntimeReadonlyDictionary::Ptr TextureImportSettingsProvider::getAssetImportSettings(const io::FsPath& assetPath, [[maybe_unused]] const IAssetContainerLoader& loader)
    {
        auto& fileSystem = getServiceProvider().get<io::IFileSystem>();

        const io::FsPath metaPath = io::FsPath(assetPath + ".meta");
        if(!fileSystem.exists(metaPath, io::FsEntryKind::File))
        {
            return nullptr;
        }

        auto file = fileSystem.openFile(metaPath, {io::AccessMode::Read, io::AccessMode::Async}, io::OpenFileMode::OpenExisting);
        NAU_ASSERT(file);
        if(!file)
        {
            return nullptr;
        }

        io::IStreamReader::Ptr stream = file->createStream();

        Result<RuntimeValue::Ptr> jsonValue = serialization::jsonParse(*stream);
        if(!jsonValue)
        {
            NAU_LOG_WARNING("Invalid .meta file ({}):({})", metaPath.getString(), jsonValue.getError()->getDiagMessage());
            return nullptr;
        }

        return *jsonValue;
    }
}  // namespace nau
